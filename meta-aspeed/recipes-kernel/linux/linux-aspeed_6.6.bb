# Copyright (c) Meta Platforms, Inc. and affiliates. (http://www.meta.com)
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
#

include linux-aspeed.inc
require recipes-kernel/linux/linux-yocto.inc
require common/recipes-kernel/linux/linux-patches-6.6.inc

KBRANCH = "dev-6.6"
KSRC ?= "git://github.com/openbmc/linux;protocol=https;branch=${KBRANCH}"
SRC_URI += "${KSRC}"

SRCREV = "bf74ee101d3403d72dac5997be083ac41028c66b"
LINUX_VERSION ?= "6.6.43"
LINUX_VERSION_EXTENSION ?= "-aspeed"
PR = "r1"
PV = "${LINUX_VERSION}"
LIC_FILES_CHKSUM = "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"

#
# Include common defconfig file
#
SRC_URI += "file://defconfig-6.6/${SOC_MODEL}/defconfig"

do_kernel_configme[depends] += "virtual/${TARGET_PREFIX}gcc:do_populate_sysroot"
KCONFIG_MODE="--alldefconfig"

S = "${WORKDIR}/git"
