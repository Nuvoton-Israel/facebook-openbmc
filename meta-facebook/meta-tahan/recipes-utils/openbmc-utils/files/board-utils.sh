#!/bin/bash
#
# Copyright (c) Meta Platforms, Inc. and affiliates.
#
# This program file is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; version 2 of the License.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program in a file named COPYING; if not, write to the
# Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301 USA

PWRCPLD_SYSFS_DIR=$(i2c_device_sysfs_abspath 12-0060)

CHASSIS_POWER_CYCLE="${PWRCPLD_SYSFS_DIR}/power_cycle_go"
BOARD_ID="${PWRCPLD_SYSFS_DIR}/board_id"
VERSION_ID="${PWRCPLD_SYSFS_DIR}/version_id"

wedge_board_type() {
    echo 'tahan'
}

wedge_board_rev() {
    board_id=$(head -n 1 < "$BOARD_ID" 2> /dev/null)
    version_id=$(head -n 1 < "$VERSION_ID" 2> /dev/null)

    case "$((board_id))" in
        1)
            echo "Board type: Tahan Switching System"
            ;;
        *)
            echo "Board type: unknown value [$board_id]"
            ;;
    esac

    case "$((version_id))" in
        0)
            echo "Revision: EVT-1"
            ;;
        *) 
            echo "Revision: unknown value [$version_id]"
            ;;
    esac

    return 0
}

userver_power_is_on() {
    echo "FIXME: feature not implemented!!"
    return 1
}

userver_power_on() {
    echo "FIXME: feature not implemented!!"
    return 1
}

userver_power_off() {
    echo "FIXME: feature not implemented!!"
    return 1
}

userver_reset() {
    return 1
    echo "FIXME: feature not implemented!!"
}

chassis_power_cycle() {
    echo 1 > "$CHASSIS_POWER_CYCLE"
    return 1
}

bmc_mac_addr() {
    bmc_mac=$(weutil -e chassis_eeprom | grep 'Local MAC:' | cut -d ' ' -f 3)
    echo "$bmc_mac"
}

#
# SCM EEPROM doesn't exist in Tahan, and CPU MAC address is obtained by
# adding 1 to the BMC MAC address.
#
userver_mac_addr() {
    bmc_mac=$(bmc_mac_addr)
    cpu_mac=$(mac_addr_inc "$bmc_mac")
    echo "$cpu_mac"
}

