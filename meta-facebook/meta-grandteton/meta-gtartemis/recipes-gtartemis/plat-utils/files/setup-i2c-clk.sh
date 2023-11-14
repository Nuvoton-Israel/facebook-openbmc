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

. /usr/local/fbpackages/utils/ast-functions

GTA_MB_EVT_BORAD_ID="3"
mbrev=$(kv get mb_rev)

if [ "$mbrev" -le "$GTA_MB_EVT_BORAD_ID" ]
then
	# set MEB BIC i2c clk frequency to 100K
	$DEVMEM 0x1e78a504 32 0x00CDF003
else
	# set MEB BIC i2c clk frequency to 400K and enlarge the SDA hold time
	$DEVMEM 0x1e78a504 32 0x0867E602
fi
