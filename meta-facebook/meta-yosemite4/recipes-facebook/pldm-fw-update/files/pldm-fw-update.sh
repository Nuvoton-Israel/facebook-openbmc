#!/bin/bash

# shellcheck source=meta-facebook/meta-yosemite4/recipes-yosemite4/plat-tool/files/yosemite4-common-functions
source /usr/libexec/yosemite4-common-functions

RETRY_UPDATE_COUNT=200
MAX_RETRIES=3
lockfile="/tmp/pldm-fw-update.lock"

exec 200>"$lockfile"
retry_remain_count=$RETRY_UPDATE_COUNT
echo ""
while true
do
    exec 200>"$lockfile"
    if ! flock -n 200; then
        echo -ne "PLDM firmware update is already running, retry in 10 seconds, $retry_remain_count time(s) remaining...  "\\r
        retry_remain_count=$((retry_remain_count - 1))
        if [ $retry_remain_count -eq 0 ]; then
            echo ""
			echo "Retry time exhausted, abort"
            exit 1
        fi
        sleep 10
    else
        break
    fi
done

SB_CPLD_ADDR="0x22"
SB_UART_MUX_SWITCH_REG="0x08"
BIC_BOOT_STRAP_SPI_VAL="0x00"
BIC_BOOT_STRAP_REG="0x0C"

SD_EEPROM_ADDR="0x54"
S3_REMAINING_WRTIE_OFFSET=("0x09" "0x00")
CPU0_REMAINING_WRTIE_OFFSET=("0x09" "0x02")
CPU1_REMAINING_WRTIE_OFFSET=("0x09" "0x04")

RAA229620_RAA229621_RW_TIMES=28
MP2857_TPS536C5_RW_TIMES=1000

VR_TYPE=""
VR_OFFSET=""
VR_HIGHBYTE_OFFSET=""
VR_LOWBYTE_OFFSET=""
remaining_write_times=""

# For firmware update
# PLDM update
FAIL_TO_UPDATE_PLDM_TIME_OUT_ERROR=22
FAIL_TO_UPDATE_PLDM_FAIL_TO_DELETE_SOFTWARE_ID=23
FAIL_TO_UPDATE_PLDM_IMAGE_DOES_NOT_MATCH=24
FAIL_TO_UPDATE_PLDM_MISS_SOFTWARE_ID=25
FAIL_TO_UPDATE_PLDM_FAIL_TO_INIT_REAMAINING_WRITE_UNKNOWN_VR_DEVICE=28
FAIL_TO_UPDATE_PLDM_SD_VR_SLOT_ID_IS_WRONG=30
FAIL_TO_UPDATE_PLDM_UNABLE_TO_DETERMINE_RETIMER_TYPE=33
NO_RETIMER_ON_THE_FRU=34
FAIL_TO_UPDATE_PLDM_PLDM_SOFTWARE_ACTIVATION_STATUS_FAIL=35
FAIL_TO_UPDATE_PLDM_MCTP_EID_NOT_EXIST=36


MANAGEMENT_BOARD_IO_EXP_BUS_NUM="34"
MANAGEMENT_BOARD_VERSION=$(get_product_version Management_Board)
if [ "$MANAGEMENT_BOARD_VERSION" = "DVT" ] || [ "$MANAGEMENT_BOARD_VERSION" = "EVT" ]; then
	MANAGEMENT_BOARD_IO_EXP_BUS_NUM="13"
fi

is_nuvoton_board="$(check_nuvoton_board)"

show_usage() {
	echo "Usage: pldm-fw-update.sh [sd|ff|wf|sd_vr|wf_vr|sd_retimer] (--rcvy <slot_id> <uart image>) (<slot_id>) <pldm image>"
	echo "       update all PLDM component  : pldm-fw-update.sh [sd|ff|wf] <pldm image>"
	echo "       update one PLDM component  : pldm-fw-update.sh [sd|ff|wf|sd_vr|wf_vr|sd_retimer] <slot_id> <pldm image>"
	echo "       recover and then update one BIC. : pldm-fw-update.sh [sd|ff|wf] --rcvy <slot_id> <uart image> <pldm image>"
	echo "       recovery SD re-timer : pldm-fw-update.sh [sd_retimer] <slot_id> <pldm recovery image>"
	echo ""
}

check_power_on() {
	local slot=$1
	local i2c_bus=$((slot-1))
	local power_status=0

	power_status=$(gpioget "$(basename /sys/bus/i2c/devices/"$i2c_bus"-0023/*gpiochip*)" 16)
	if [ "$power_status" != "1" ]; then
		echo "Check Power : Off"
		echo "Power on before FF/WF PLDM component update"
		exit 255
	fi

	echo "Check Power : On"
}

recovery_bic_by_uart() {
	local slot=$1
	local cpld_uart_routing=$2
	local boot_strap_reg=$3
	local uart_image=$4
	local uart_num=$((slot < 5 ? slot - 1 :slot))
	local i2c_bus=$((slot-1))
	local ret=255

	echo "UART is ttyS$uart_num"
	echo "i2c bus is $i2c_bus"

	# Switch UART to corresponding BIC
	i2ctransfer -f -y "$i2c_bus" w2@"$SB_CPLD_ADDR" "$SB_UART_MUX_SWITCH_REG" "$cpld_uart_routing"
	ret=$?
	if [ "$ret" -ne 0 ]; then
		return $ret
	fi

	# Set BIC boot from UART
	echo "Setting BIC boot from UART"
	i2ctransfer -f -y "$i2c_bus" w2@"$SB_CPLD_ADDR" "$BIC_BOOT_STRAP_REG" "$boot_strap_reg"
	ret=$?
	if [ "$ret" -ne 0 ]; then
		return $ret
	fi

	# Notify Nuvoton MGM CPLD to change baud rate to 115200
	if [ -n "$is_nuvoton_board" ]; then
		if [ "$slot" -ge 5 ] && [ "$slot" -le 8 ]; then
			set_val=$((0x1 << (slot-1)))
			i2ctransfer -f -y $MANAGEMENT_BOARD_IO_EXP_BUS_NUM w2@0x21 0x0C "$set_val"
		fi
	fi

	# set UART to 115200
	/bin/stty -F "/dev/ttyS$uart_num" 115200
	ret=$?
	if [ "$ret" -ne 0 ]; then
		return $ret
	fi

	sleep 3

	echo "Doing the recovery update..."

	#echo "$uart_image"
	cat "$uart_image" > "/dev/ttyS$uart_num"

	sleep 5

	# Notify Nuvoton MGM CPLD to change baud rate to 57600
	if [ -n "$is_nuvoton_board" ]; then
		if [ "$slot" -ge 5 ] && [ "$slot" -le 8 ]; then
			i2ctransfer -f -y $MANAGEMENT_BOARD_IO_EXP_BUS_NUM w2@0x21 0x0C 0x0
		fi
	fi

	# set UART back to 57600
	/bin/stty -F "/dev/ttyS$uart_num" 57600
	ret=$?
	if [ "$ret" -ne 0 ]; then
		return $ret
	fi

	# Set BIC boot from spi
	i2ctransfer -f -y "$i2c_bus" w2@"$SB_CPLD_ADDR" "$BIC_BOOT_STRAP_REG" "$BIC_BOOT_STRAP_SPI_VAL"
	ret=$?
	if [ "$ret" -ne 0 ]; then
		return $ret
	fi

	echo "Recovery BIC is finished."
	return 0
}

wait_for_update_complete() {
	local counter=5
	while true
	do
		sleep 5
		echo -ne "Waiting for updating... ${counter} sec"\\r

		# Capture the output of busctl command
		local output
		output=$(busctl get-property xyz.openbmc_project.PLDM /xyz/openbmc_project/software/"$software_id" xyz.openbmc_project.Software.ActivationProgress Progress --timeout=120 2>&1)
		ret=$?
		if [ "$ret" -ne "0" ]; then
			local status

			if echo "$output" | grep -q "Unknown object"; then
				echo "The software ID $software_id does not exist."
				return $FAIL_TO_UPDATE_PLDM_MISS_SOFTWARE_ID
			fi

			status=$(busctl get-property xyz.openbmc_project.PLDM /xyz/openbmc_project/software/"$software_id" xyz.openbmc_project.Software.Activation Activation --timeout=120 2>&1 )
			status=$(echo "$status" | cut -d " " -f 2)
			if [ "${status}" == "xyz.openbmc_project.Software.Activation.Activations.Failed" ] || [ "${status}" == "xyz.openbmc_project.Software.Activation.Activations.Invalid" ]; then
				echo -ne \\n"PLDM software activation is ${status}."\\n
				return $FAIL_TO_UPDATE_PLDM_PLDM_SOFTWARE_ACTIVATION_STATUS_FAIL
			fi
		fi

		# Extract progress value from the output
		local progress
		progress=$(echo "$output" | cut -d " " -f 2)

		if [ "${progress}" == 100 ]; then
			echo -ne \\n"Update done."\\n
			return 0
		fi

		counter=$((counter+5))
		# Over five minutes is considered timeout
		if [ "${counter}" == 300 ]; then
			echo -ne \\n"Time out. Fail"\\n
			return $FAIL_TO_UPDATE_PLDM_TIME_OUT_ERROR
		fi
	done
}

delete_software_id() {
	local counter=0
	if [ "$software_id" != "" ]; then
		while true
		do
			sleep 2
			activation=$(busctl  get-property xyz.openbmc_project.PLDM /xyz/openbmc_project/software/"$software_id" xyz.openbmc_project.Software.Activation Activation --timeout=120 2>&1 | cut -d " " -f 2)
			#Check the status of pldm updater, if the status is in activating, we cannot call the delete method
			if [ "${activation}" != "\"xyz.openbmc_project.Software.Activation.Activations.Activating\"" ]; then
				echo "Delete software id. software id = $software_id"
				output=$(busctl call xyz.openbmc_project.PLDM /xyz/openbmc_project/software/"$software_id" xyz.openbmc_project.Object.Delete Delete --timeout=120 2>&1)
				local ret=$?
				if [ "$ret" -ne 0 ]; then
					echo "Failed to delete software id. Return with error code: $FAIL_TO_UPDATE_PLDM_FAIL_TO_DELETE_SOFTWARE_ID. Error message: $output"
					return $FAIL_TO_UPDATE_PLDM_FAIL_TO_DELETE_SOFTWARE_ID
				fi
				break
			fi
			counter=$((counter+2))
			if [ "${counter}" == 10 ]; then
				echo "Object /xyz/openbmc_project/software/$software_id is stalled in activating"
				echo "Restarting pldmd to recover it."
				systemctl stop pldmd
				sleep 10
				systemctl start pldmd
				sleep 60
				return $FAIL_TO_UPDATE_PLDM_FAIL_TO_DELETE_SOFTWARE_ID
			fi
		done
	fi
}

update_bic() {
	cp "$1" /tmp/pldm_images
	sleep 6

	echo "Generating software ID..."
	software_id=$(compute-software-id "$1")
	echo "software_id = $software_id"

	if [ "$software_id" != "" ]; then
		if ! busctl get-property xyz.openbmc_project.PLDM /xyz/openbmc_project/software/"$software_id" xyz.openbmc_project.Software.ActivationProgress Progress --timeout=120 >/dev/null 2>&1; then
			echo "The image does not match with any devices. Return with error code: $FAIL_TO_UPDATE_PLDM_IMAGE_DOES_NOT_MATCH"
			return $FAIL_TO_UPDATE_PLDM_IMAGE_DOES_NOT_MATCH
		fi
		sleep 10
		busctl set-property xyz.openbmc_project.PLDM /xyz/openbmc_project/software/"$software_id" xyz.openbmc_project.Software.Activation RequestedActivation s "xyz.openbmc_project.Software.Activation.RequestedActivations.Active"
		wait_for_update_complete
		update_status=$?
		if [ "$3" == "wf_bic" ] || [ "$3" == "sd_bic" ]; then
			bic_eid="${2}0"
			if [ "$3" == "wf_bic" ]; then
				bic_eid=$((bic_eid+2))
			fi
			if [ $update_status == $FAIL_TO_UPDATE_PLDM_TIME_OUT_ERROR ]; then
				local output
				output=$(pldmtool fw_update GetStatus -m "${bic_eid}")
				bic_current_state=$(echo "$output" | grep -o '"CurrentState": "[^"]*' | cut -d'"' -f4)
				if [ "$bic_current_state" = "DOWNLOAD" ]; then
				pldmtool raw -d 0x80 0x3f 0x1 0x15 0xa0 0x0 0x18 0x03 -m "${bic_eid}"
				sleep 6 # wait for BIC reset
				fi
			fi
		fi
		return $update_status
	else
		echo "Fail: Miss software id."
		return $FAIL_TO_UPDATE_PLDM_MISS_SOFTWARE_ID
	fi
}


update_all_slots() {
	local is_recovery=$1
	local user_specific_slot_id=$2

	if [ "$is_recovery" == false ] && [ -z "$user_specific_slot_id" ]; then
		# 0 = true
		return 0
	else
		return 1
	fi
}

update_single_slot() {
	local slot_id=$1
	local user_specific_slot_id=$2
	local is_recovery=$3

	if [ "$is_recovery" == false ] && [ "$slot_id" == "$user_specific_slot_id" ]; then
		# 0 = true
		return 0
	else
		return 1
	fi
}

conduct_normal_update_and_updated_slot() {
	local slot_id=$1
	local user_specific_slot_id=$2
	local is_recovery=$3

	if update_single_slot "$slot_id" "$user_specific_slot_id" "$is_recovery" ; then
		# 0 = true
		return 0
	elif update_all_slots "$is_recovery" "$user_specific_slot_id"; then
		return 0
	else
		return 1
	fi
}

conduct_recovery_and_not_recovered_slot() {
	local slot_id=$1
	local user_specific_slot_id=$2
	local is_recovery=$3

	if [ "$is_recovery" == true ] && [ "$slot_id" != "$user_specific_slot_id" ]; then
		# 0 = true
		return 0
	else
		return 1
	fi
}

is_sd_bic() {
	local slot_id=$1
	local mctp_exp_id=$2

	if [[ "$slot_id" -ge 1 && "$slot_id" -le 8 && "$mctp_exp_id" == "0" ]]; then
		# 0 = true
		return 0
	else
		return 1
	fi
}


is_other_bic_updating() {
  echo "Check if other PLDM components are updating"

  retry_remain_count=$RETRY_UPDATE_COUNT
  while true
  do
	  pldm_output=$(busctl tree xyz.openbmc_project.PLDM)
	  if echo "$pldm_output" | grep -qE "/xyz/openbmc_project/software/[0-9]+"; then
		echo "$pldm_output" | grep -E "/xyz/openbmc_project/software/[0-9]+"
		previous_software_id=$(busctl tree xyz.openbmc_project.PLDM |grep /xyz/openbmc_project/software/ | cut -d "/" -f 5)
		busctl get-property xyz.openbmc_project.PLDM /xyz/openbmc_project/software/"$previous_software_id" xyz.openbmc_project.Software.ActivationProgress Progress > /dev/null 2>&1
		ret=$?
		if [ "$ret" -eq 0 ]; then
			echo -ne "Please wait until the other software update is completed, retry in 10 seconds, $retry_remain_count time(s) remaining... "\\r
			retry_remain_count=$((retry_remain_count - 1))
			if [ $retry_remain_count -eq 0 ]; then
				echo ""
				echo "Retry time exhausted, abort"
				exit 255
			fi
		else
			break
		fi
	  else
		break
	  fi
	  sleep 10
  done
}

# Function to prompt for continuation and check user input
prompt_confirmation() {
	echo "WARNING! This will automatically update all PLDM components."
	read -r -p "Continue? [y/N] " continue
	if [ "$continue" != "y" ]; then
		echo "Aborting on user request."
		exit 0
	fi
}

# Function to display an error message and exit the script
error_and_exit() {
	echo "Please provide $1 image"
	show_usage
	exit 255
}

get_offset_for_vr_type() {
    local vr_type=$1
    local S3_VR_ID="02"
    local CPU0_VR_ID="03"
    local CPU1_VR_ID="01"

    case "$vr_type" in
        *"$S3_VR_ID")
            printf "%s\n" "${S3_REMAINING_WRTIE_OFFSET[@]}"
            ;;
        *"$CPU0_VR_ID")
            printf "%s\n" "${CPU0_REMAINING_WRTIE_OFFSET[@]}"
            ;;
        *"$CPU1_VR_ID")
            printf "%s\n" "${CPU1_REMAINING_WRTIE_OFFSET[@]}"
            ;;
        *)
            echo "Failed to initialize remaining write because of unknown VR type"
            exit 255
            ;;
    esac
}

Initialize_vr_remaining_write() {
    local slot_id=$1
    local eeprom_addr=$2
    local pldm_image=$3
    local hex_dump

    hex_dump=$(hexdump -c "$pldm_image" | head -n 15 | cut -c 9- | tr -d '\n[:space:]')

    case "$hex_dump" in
        *"raa229620"*|*"raa229621"*)
            remaining_write_times=$RAA229620_RAA229621_RW_TIMES
            ;;
        *"mp2857"*|*"tps536c5"*|*"mp2856"*)
            remaining_write_times=$MP2857_TPS536C5_RW_TIMES
            ;;
        *)
            echo "Failed to initialize remaining write because of unknown VR device"
            exit $FAIL_TO_UPDATE_PLDM_FAIL_TO_INIT_REAMAINING_WRITE_UNKNOWN_VR_DEVICE
            ;;
    esac

    set_vr_remaining_write_to_eeprom "$slot_id" "$eeprom_addr" "$remaining_write_times"
}

check_vr_remaining_write() {
    local i2c_bus=$(($1+15))
    local slot_id=$1
    local eeprom_addr=$2
    local pldm_image=$3
    local result

    i2ctransfer -f -y $i2c_bus w2@"$eeprom_addr" "$VR_HIGHBYTE_OFFSET" "$VR_LOWBYTE_OFFSET"

    result=$(i2ctransfer -f -y  $i2c_bus r2@"$eeprom_addr")

    IFS=' ' read -ra result_array <<< "$result"
    result="0x${result_array[0]#0x}${result_array[1]#0x}"
    remaining_write_times=$(printf "%d" "$result")

    if [ "$remaining_write_times" -eq 0 ]; then
        echo "VR device can't be updated because the remaining write times is 0"
        exit 255
    fi
}

set_vr_remaining_write_to_eeprom() {
	local i2c_bus=$(($1+15))
	local eeprom_addr=$2
	local rw_times=$3
	local hex

	hex=$(printf "%04x" "$rw_times")
	# Split the hex into two parts
	local hex1=${hex:0:2}
	local hex2=${hex:2}
	# Add the prefix "0x" to each part
	local byte1="0x$hex1"
	local byte2="0x$hex2"
	# Write the remaining write times back to EEPROM
	i2ctransfer -f -y $i2c_bus w4@"$eeprom_addr" "$VR_HIGHBYTE_OFFSET" "$VR_LOWBYTE_OFFSET" "$byte1" "$byte2"
}

check_if_no_retimer_sku() {
    local slot_id=$1

    # retimer 1 CHIP GPIO 97
    gpio_97=$(pldmtool raw -d 0x80 0x02 0x3a 0x61 0xFF -m "${slot_id}0" 2>/dev/null)
    # echo "gpio_97 = $gpio_97"
    gpio_97_rx_data=$(echo "$gpio_97" | grep 'Rx:' | cut -d':' -f2-)
    # echo "gpio_97_rx_data = $gpio_97_rx_data"
    gpio_97_cc_byte=$(echo "$gpio_97_rx_data" | awk '{print $(NF-7)}')
    # echo "gpio_97_cc_byte = $gpio_97_cc_byte"
    gpio_97_last_byte=$(echo "$gpio_97_rx_data" | awk '{print $NF}' )
    # echo "gpio_97_last_byte = $gpio_97_last_byte"
    # retimer 0 CHIP GPIO 95
    gpio_95=$(pldmtool raw -d 0x80 0x02 0x3a 0x5f 0xFF -m "${slot_id}0" 2>/dev/null)
    # echo "gpio_95 = $gpio_95"
    gpio_95_rx_data=$(echo "$gpio_95" | grep 'Rx:' | cut -d':' -f2-)
    # echo "gpio_95_rx_data = $gpio_95_rx_data"
    gpio_95_cc_byte=$(echo "$gpio_95_rx_data" | awk '{print $(NF-7)}')
    # echo "gpio_95_cc_byte = $gpio_95_cc_byte"
    gpio_95_last_byte=$(echo "$gpio_95_rx_data" | awk '{print $NF}' )
    # echo "gpio_95_last_byte = $gpio_95_last_byte"

    # pldm tool raw command failed
    if [ "$gpio_97_cc_byte" != "00" ] || [ "$gpio_95_cc_byte" != "00" ]; then
        echo "Retimer update failed, unable to determine retimer type."
        return $FAIL_TO_UPDATE_PLDM_UNABLE_TO_DETERMINE_RETIMER_TYPE
    fi

    # 0102==retimer[1:0] get 01, means no retimer on the FRU.
    retimer_type="${gpio_97_last_byte}${gpio_95_last_byte}"
    # echo "retimer_type = $retimer_type"
    if [ "$retimer_type" == "0102" ]; then
        return $NO_RETIMER_ON_THE_FRU
    fi

    return 0
}

restart_sd_bic_for_i3c_re-init () {
	# Workaround: Restart all Sentinel dome BIC for i3c hub re-init
	mapfile -t eid_arr < <(busctl tree xyz.openbmc_project.MCTP | grep /xyz/openbmc_project/mctp/1/ | cut -d "/" -f 6)
	for EID in "${eid_arr[@]}"
	do
		mctp_slot_id=$((EID/10));
		mctp_exp_id=$((EID%10));

		if is_sd_bic "$mctp_slot_id" "$mctp_exp_id"; then
			if conduct_recovery_and_not_recovered_slot "$mctp_slot_id" "$slot_id" "$is_rcvy" ||
			conduct_normal_update_and_updated_slot "$mctp_slot_id" "$slot_id" "$is_rcvy"; then
				echo "Slot $mctp_slot_id: Restart BIC"
				pldmtool raw -m "$mctp_slot_id"0 -d 0x80 0x02 0x39 0x1 0x1 0x1 0x1 0x1
			fi
		fi
	done
	sleep 5
}

handle_firmware_operations () {
	# Execute recovery operations if in recovery mode, based on the value of bic_name
	echo "Start processing firmware operations for slot $slot_id using PLDM."
	if [ "$is_rcvy" == true ]; then

		is_other_bic_updating

		# Workaround: Avoid pldm daemon blocked
		systemctl stop pldmd
		echo "Start to Recovery slot $slot_id $bic_name BIC"
		case $bic_name in
		sd)
			recovery_bic_by_uart "$slot_id" "0x04" "0x01" "$uart_image"
			ret=$?
			;;
		ff|wf)
			check_power_on "$slot_id"
			cpld_uart_routing=$( [ "$bic_name" == "ff" ] && echo "0x05" || echo "0x01" )
			boot_strap_reg=$( [ "$bic_name" == "ff" ] && echo "0x04" || echo "0x02" )
			recovery_bic_by_uart "$slot_id" "$cpld_uart_routing" "$boot_strap_reg" "$uart_image"
			ret=$?
			pldmtool raw -m "${slot_id}0" -d 0x80 0x02 0x39 0x1 0x1 0x1 0x1 0x1
			;;
		esac

		if [ "$ret" -ne 0 ]; then
			echo "Failed to Recovery BIC. Exiting with error code: $ret"
			exit "$ret"
		fi

		echo "Restart MCTP service and PLDM service"
		sleep 3
		systemctl restart mctpd
		sleep 20
		systemctl start pldmd
		sleep 60
	fi

	# TODO: WF/FF need to implement the remaining write times check
	if [ "$bic_name" == "sd_vr" ]; then
		if ! [[ "$slot_id" =~ ^[1-8]$ ]]; then
			echo "Failed to update SD VR because <slot${slot_id}> is wrong"
			exit $FAIL_TO_UPDATE_PLDM_SD_VR_SLOT_ID_IS_WRONG
		fi
		check_vr_remaining_write "$slot_id" "$SD_EEPROM_ADDR" "$pldm_image"
		echo "remaining write before updating: $remaining_write_times"
	fi

	if ! systemctl is-active --quiet pldmd; then
		echo "STOP. PLDM service is not running. Please check pldmd status."
		exit 255
	fi

	if [ "$is_rcvy" == true ]; then
		pldm-package-re-wrapper bic -s "$slot_id" -f "$pldm_image"
		pldm_image="${pldm_image}_re_wrapped"
		echo "PLDM image name: ${pldm_image}"
	fi

	mctp_tree=$(busctl tree xyz.openbmc_project.MCTP | tee /dev/tty)
	case "$bic_name" in
	sd|sd_vr|sd_retimer)
		if [ "$is_rcvy" == false ] && ! echo "$mctp_tree" | grep -q "/xyz/openbmc_project/mctp/1/${slot_id}0"; then
			echo "EID ${slot_id}0 does not exist, unable to update PLDM device. Return with error code: $FAIL_TO_UPDATE_PLDM_MCTP_EID_NOT_EXIST"
			return $FAIL_TO_UPDATE_PLDM_MCTP_EID_NOT_EXIST
		fi
		;;
	wf|wf_vr)
		if [ "$is_rcvy" == false ] && ! echo "$mctp_tree" | grep -q "/xyz/openbmc_project/mctp/1/${slot_id}2"; then
			echo "EID ${slot_id}2 does not exist, unable to update PLDM device. Return with error code: $FAIL_TO_UPDATE_PLDM_MCTP_EID_NOT_EXIST"
			return $FAIL_TO_UPDATE_PLDM_MCTP_EID_NOT_EXIST
		fi
		;;
	esac
	echo "Start to Update PLDM component"

	is_other_bic_updating

	update_bic "$pldm_image" "$slot_id" "$bic_name"
	ret=$?
	if [ "$ret" -ne 0 ]; then
		echo "Failed to Update PLDM component. Return with error code: $ret"
		delete_software_id
		echo "Restart PLDM service for recover"
		systemctl stop pldmd
		sleep 10
		systemctl start pldmd
		sleep 60
		return $ret
	fi

	sleep 3

	if [ "$is_rcvy" == true ]; then
		echo "Slot$slot_id: Do 12V cycle"
		busctl set-property xyz.openbmc_project.State.Chassis"$slot_id" /xyz/openbmc_project/state/chassis"$slot_id" xyz.openbmc_project.State.Chassis RequestedPowerTransition s "xyz.openbmc_project.State.Chassis.Transition.PowerCycle"
		sleep 40
	else
		if [ "$bic_name" == "wf" ] || [ "$bic_name" == "ff" ]; then
			restart_sd_bic_for_i3c_re-init
		elif [ "$bic_name" == "sd" ]; then
			sleep 15
			restart_sd_bic_for_i3c_re-init
		fi
	fi

	if [ "$bic_name" == "sd_vr" ]; then
		i2c_bus=$((slot_id+15))
		i2ctransfer -f -y $i2c_bus w2@"$SD_EEPROM_ADDR" "$VR_HIGHBYTE_OFFSET" "$VR_LOWBYTE_OFFSET"
		result=$(i2ctransfer -f -y  $i2c_bus r2@"$SD_EEPROM_ADDR")
		if [ "$result" == "0xff 0xff" ]; then
			Initialize_vr_remaining_write "$slot_id" "$SD_EEPROM_ADDR" "$pldm_image"
		fi
		remaining_write_times=$((remaining_write_times-1))
		set_vr_remaining_write_to_eeprom "$slot_id" "$SD_EEPROM_ADDR" "$remaining_write_times"
		echo "remaining write after updating: $remaining_write_times"
	fi

	delete_software_id

	# Update BIC version to Settings D-Bus
	if [ "$bic_name" == "sd" ]; then
		echo "Updating SD BIC version to Settings D-Bus"
		sleep 15 # wait for BIC reset
		# Check if slot_id is empty
		if [ -z "$slot_id" ]; then
			# Loop through slot_id values from 1 to 8 if slot_id is empty
			for slot_id in {1..8}; do
				# Check EID presence first
				busctl get-property xyz.openbmc_project.MCTP \
					/xyz/openbmc_project/mctp/1/"$slot_id"0 \
					xyz.openbmc_project.MCTP.Endpoint EID  > /dev/null 2>&1
				ret=$?
				if [ "$ret" -eq 0 ]; then
					/usr/libexec/fw-versions/sd-bic "$slot_id"
					ret=$?
					# Check if the command was successful
					if [ "$ret" -eq 0 ]; then
						version=$(busctl get-property xyz.openbmc_project.Settings \
							"/xyz/openbmc_project/software/host$slot_id/Sentinel_Dome_bic" \
							xyz.openbmc_project.Software.Version Version | awk -F'"' '{print $2}')
						echo "Version retrieved successfully: $version"
					fi
				fi
			done
		else
			# Execute the command with the provided slot_id
			/usr/libexec/fw-versions/sd-bic "$slot_id"
			ret=$?
			# Check if the command was successful
			if [ "$ret" -eq 0 ]; then
				version=$(busctl get-property xyz.openbmc_project.Settings \
					"/xyz/openbmc_project/software/host$slot_id/Sentinel_Dome_bic" \
					xyz.openbmc_project.Software.Version Version | awk -F'"' '{print $2}')
				echo "Version retrieved successfully: $version"
			fi
		fi
	elif [ "$bic_name" == "wf" ]; then
		echo "Updating WF BIC version to Settings D-Bus"
		sleep 15 # wait for BIC reset
		# Check if slot_id is empty
		if [ -z "$slot_id" ]; then
			# Loop through slot_id values from 1 to 8 if slot_id is empty
			for slot_id in {1..8}; do
				# Check EID presence first
				busctl get-property xyz.openbmc_project.MCTP \
					/xyz/openbmc_project/mctp/1/"$slot_id"2 \
					xyz.openbmc_project.MCTP.Endpoint EID  > /dev/null 2>&1
				ret=$?
				if [ "$ret" -eq 0 ]; then
					/usr/libexec/fw-versions/wf-bic "$slot_id"
					ret=$?
					# Check if the command was successful
					if [ "$ret" -eq 0 ]; then
						version=$(busctl get-property xyz.openbmc_project.Settings \
							"/xyz/openbmc_project/software/host$slot_id/Wailua_Falls_bic" \
							xyz.openbmc_project.Software.Version Version | awk -F'"' '{print $2}')
						echo "Version retrieved successfully: $version"
					fi
				fi
			done
		else
			# Execute the command with the provided slot_id
			/usr/libexec/fw-versions/wf-bic "$slot_id"
			ret=$?
			# Check if the command was successful
			if [ "$ret" -eq 0 ]; then
				version=$(busctl get-property xyz.openbmc_project.Settings \
					"/xyz/openbmc_project/software/host$slot_id/Wailua_Falls_bic" \
					xyz.openbmc_project.Software.Version Version | awk -F'"' '{print $2}')
				echo "Version retrieved successfully: $version"
			fi
		fi
	fi
	echo "Completed firmware operations for slot $slot_id."
	echo "Done"
	return 0
}

retry_firmware_operation() {
    local retries=0
    local ret=0

    handle_firmware_operations
    ret=$?
    if [ $ret -eq 0 ]; then
        return 0
	elif [ $ret -eq $FAIL_TO_UPDATE_PLDM_MCTP_EID_NOT_EXIST ]; then
		return $ret
    fi

    while [ $retries -lt $MAX_RETRIES ]; do
        retries=$((retries + 1))
        echo "Firmware operation for slot $slot_id failed. Retrying... ($retries/$MAX_RETRIES)"
        handle_firmware_operations
        ret=$?
        if [ $ret -eq 0 ]; then
            return 0
        fi
    done

    echo "Maximum retries ($MAX_RETRIES) reached. Exiting with error code: $ret."
    return $ret
}

# Check for minimum required arguments
[ $# -lt 2 ] && error_and_exit "PLDM"

is_rcvy=false
slot_id=
bic_name=$1
pldm_image=$2

# Determine recovery mode and check for required image files based on argument count
if [ $# -eq 5 ] && [ "$2" == "--rcvy" ]; then

	if [ "$bic_name" == "sd_vr" ] || [ "$bic_name" == "wf_vr" ]; then
		echo "VR device can't be updated in recovery mode"
		exit 255
	elif [ "$bic_name" == "sd" ]; then
		echo "Initiate SD BIC recovery"
	elif [ "$bic_name" == "wf" ]; then
		echo "Initiate WF BIC recovery"
	fi

	is_rcvy=true
	slot_id=$3
	uart_image=$4
	[ ! -f "$uart_image" ] && error_and_exit "UART"
	pldm_image=$5
	retry_firmware_operation
elif [ $# -eq 3 ] && [[ "$2" =~ ^[1-8]+$ ]]; then
	slot_id=$2
	pldm_image=$3
	if [ "$bic_name" == "sd_vr" ]; then
		VR_TYPE=$(hexdump -n 1 -s 0x6D -e '1/1 "%02x"' "$pldm_image")
		mapfile -t VR_OFFSET < <(get_offset_for_vr_type "$VR_TYPE")
		VR_HIGHBYTE_OFFSET="${VR_OFFSET[0]}"
		VR_LOWBYTE_OFFSET="${VR_OFFSET[1]}"
	elif [ "$bic_name" == "sd" ]; then
		echo "Initiate SD BIC update"
	elif [ "$bic_name" == "wf" ]; then
		echo "Initiate WF BIC update"
	fi
	pldm-package-re-wrapper bic -s "$slot_id" -f "$pldm_image"
	pldm_image="${pldm_image}_re_wrapped"
	retry_firmware_operation
elif [ $# -eq 3 ] && [ "$bic_name" == "sd_retimer" ]; then
	slot_id=$2
	check_if_no_retimer_sku "$slot_id"
	exit_code=$?
	if [ $exit_code -eq $NO_RETIMER_ON_THE_FRU ]; then
		echo "Retimer update failed, no retimer on the slot${FRU#slot}."
		exit $exit_code
	elif [ $exit_code -ne 0 ]; then
		exit $exit_code
	fi
	echo "Initiate SD Retimer update"
	pldm-package-re-wrapper bic -s "$slot_id" -f "$3"
	pldm_image="${pldm_image}_re_wrapped"
	retry_firmware_operation
else
	prompt_confirmation
	if [ "$bic_name" == "sd" ]; then
		echo "Initiate SD BIC update"
	elif [ "$bic_name" == "wf" ]; then
		echo "Initiate WF BIC update"
	fi
	for i in {1..8}; do
		slot_id=$i
		pldm_image=$2
		pldm-package-re-wrapper bic -s "$slot_id" -f "$pldm_image"
		pldm_image="${pldm_image}_re_wrapped"
		retry_firmware_operation
	done
fi

[ ! -f "$pldm_image" ] && error_and_exit "PLDM"

#unlock
flock -u 200
