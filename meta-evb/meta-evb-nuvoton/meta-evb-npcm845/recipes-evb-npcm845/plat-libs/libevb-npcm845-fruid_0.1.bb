# Copyright 2015-present Facebook. All Rights Reserved.
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

SUMMARY = "EVB-NPCM845 Fruid Library"
DESCRIPTION = "library for reading all evb-npcm845 fruids"
SECTION = "base"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://evb-npcm845_fruid.c;beginline=6;endline=18;md5=da35978751a9d71b73679307c4d296ec"

SRC_URI = "file://evb-npcm845_fruid"
S = "${WORKDIR}/evb-npcm845_fruid"

inherit meson pkgconfig

DEPENDS += "libevb-npcm845-common"
RDEPENDS:${PN} += "libevb-npcm845-common"
