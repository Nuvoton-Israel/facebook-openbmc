#!/bin/bash

lockfile="/tmp/pldm-fw-upate.lock"

FAIL_TO_UPDATE_WF_CXL_PLDM_SERVICE_NOT_RUNING=16
FAIL_TO_UPDATE_WF_CXL_EID_DOES_NOT_EXIST=17
FAIL_TO_UPDATE_WF_CXL_CAN_ONLY_UPDATE_ONE_BIC_AT_A_TIME=18
FAIL_TO_UPDATE_WF_CXL_TIME_OUT_ERROR=19
FAIL_TO_UPDATE_WF_CXL_FAIL_TO_DELETE_SOFTWARE_ID=20
INVALID_INPUT=255

exec 200>"$lockfile"
flock -n 200 || { echo "BIC update is already running"; exit 1; }

CXL1_EID_suffix=4
CXL2_EID_suffix=5

show_usage() {
    echo "Usage: cxl-fw-update.sh <slot_id> (<instance_num>) <pldm image>"
    echo "       update CXL1  : cxl-fw-update.sh <slot_id> 1 <pldm image>"
    echo "       update CXL2  : cxl-fw-update.sh <slot_id> 2 <pldm image>"
    echo ""
}

wait_for_update_complete() {
    local counter=0
    local update_success=false
    while true;
    do
        sleep 5
        echo -ne "Waiting for updating... ${counter} sec"\\r
        progress=$(busctl get-property xyz.openbmc_project.PLDM /xyz/openbmc_project/software/"$software_id" xyz.openbmc_project.Software.ActivationProgress Progress | cut -d " " -f 2)
        if [ "${progress}" == 100 ]; then
            echo -ne \\n"Update done."\\n
            update_success=true
            break
        fi
        counter=$((counter+5))
        if [ "${counter}" == 900 ]; then
            echo -ne \\n"Time out. Fail"\\n
            break
        fi
    done

    if [ "${update_success}" ]; then
        return 0
    else
        return $FAIL_TO_UPDATE_WF_CXL_TIME_OUT_ERROR
    fi
}

update_cxl() {
    local pldm_image=$1

    cp "$pldm_image" /tmp/pldm_images

    sleep 1

    software_id=$(compute-software-id "$pldm_image")
    echo "software_id = $software_id"
    sleep 1

    if [ "$software_id" != "" ]; then
        busctl set-property xyz.openbmc_project.PLDM /xyz/openbmc_project/software/"$software_id" xyz.openbmc_project.Software.Activation RequestedActivation s "xyz.openbmc_project.Software.Activation.RequestedActivations.Active"
        wait_for_update_complete
        ret=$?
        if [ "$ret" -ne 0 ]; then
            exit $ret
        fi
    else
        echo "Fail: Miss software id."
    fi
}

prompt_confirmation() {
    echo "WARNING! This will automatically update all all CXL on the blade."
    read -r -p "Continue? [y/N] " continue
    if [ "$continue" != "y" ]; then
        echo "Aborting on user request."
        exit 0
    fi
}

error_and_exit() {
    echo "Please provide valid PLDM image"
    show_usage
    exit "$INVALID_INPUT"
}

# Check for minimum required arguments
[ $# -lt 2 ] && error_and_exit

slot_id=$1
instance_num=$2
pldm_image=$3

if [ $# -eq 2 ]; then
    pldm_image=$2
fi

[ ! -f "$pldm_image" ] && error_and_exit

# Check if instance_num is between 1 and 2
if [ $# -eq 3 ] && [[ "$instance_num" =~ ^[1-2]$ ]]; then
    pldm-package-re-wrapper cxl -s "$slot_id" -i "$instance_num" -f "$pldm_image"
    pldm_image="${pldm_image}_re_wrapped"
else
    echo "Instance number must between 1 and 2."
    exit "$INVALID_INPUT"
fi

if ! systemctl is-active --quiet pldmd; then
    echo "STOP. PLDM service is not running. Please check pldmd status."
    exit "$FAIL_TO_UPDATE_WF_CXL_PLDM_SERVICE_NOT_RUNING"
fi

#Show all mctp eids, and check cxl eid should exist
#The cxl eid would end with 4 or 5.
mctp_output=$(busctl tree xyz.openbmc_project.MCTP)
echo "Check MCTP EID"
echo "$mctp_output"
if ! echo "$mctp_output" | grep -qE "/xyz/openbmc_project/mctp/1/$((slot_id * 10 + (instance_num == 1 ? CXL1_EID_suffix : CXL2_EID_suffix)))"; then
  echo "Not allowed. The CXL EID $((slot_id * 10 + (instance_num == 1 ? CXL1_EID_suffix : CXL2_EID_suffix))) does not exist."
  exit "$FAIL_TO_UPDATE_WF_CXL_EID_DOES_NOT_EXIST"
fi


echo "Check if other BICs are updating"
pldm_output=$(busctl tree xyz.openbmc_project.PLDM)
if echo "$pldm_output" | grep -qE "/xyz/openbmc_project/software/[0-9]+"; then
echo "$pldm_output" | grep -E "/xyz/openbmc_project/software/[0-9]+"
previous_software_id=$(busctl tree xyz.openbmc_project.PLDM |grep /xyz/openbmc_project/software/ | cut -d "/" -f 5)
busctl get-property xyz.openbmc_project.PLDM /xyz/openbmc_project/software/"$previous_software_id" xyz.openbmc_project.Software.ActivationProgress Progress > /dev/null
ret=$?
    if [ "$ret" -eq 0 ]; then
        echo "It can only be updated one BIC at a time. Please wait until the software update is completed."
        exit "$FAIL_TO_UPDATE_WF_CXL_CAN_ONLY_UPDATE_ONE_BIC_AT_A_TIME"
    fi
fi


update_cxl "$pldm_image"

busctl call xyz.openbmc_project.PLDM /xyz/openbmc_project/software/"$software_id" xyz.openbmc_project.Object.Delete Delete
ret=$?
if [ "$ret" -ne 0 ]; then
echo "Failed to delete software id: Exit code $ret"
exit "$FAIL_TO_UPDATE_WF_CXL_FAIL_TO_DELETE_SOFTWARE_ID"
fi

echo "Done. If there was no error appeared, please conduct DC cycle to load the new firmware."

#unlock
flock -u 200
