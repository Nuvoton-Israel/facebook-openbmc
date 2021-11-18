#!/bin/bash
#
# Copyright 2021-present Facebook. All Rights Reserved.
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

function dpe_status(){
  regVal="$1"
  dpeResultStr="$2"
  if [ "$(head -n 1 /sys/bus/i2c/drivers/scmcpld/12-0043/dpe_image)" != "$regVal" ];
  then
    echo "${dpeResultStr} image detected. exiting..."
    exit 1
  fi
}

case "$1" in
    dpe)
        dpe_status "0x1" "dpe image"
        ;;
    nondpe)
        dpe_status "0x0" "non dpe image"
        ;;
esac
