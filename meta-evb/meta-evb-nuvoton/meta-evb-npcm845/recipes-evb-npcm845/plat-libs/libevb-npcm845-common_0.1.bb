# Copyright 2015-present Facebook. All Rights Reserved.
SUMMARY = "EVB-NPCM845 Common Library"
DESCRIPTION = "library for common evb-npcm845 information"
SECTION = "base"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://evb-npcm845_common.c;beginline=8;endline=20;md5=da35978751a9d71b73679307c4d296ec"

SRC_URI = "file://evb-npcm845_common"
S = "${WORKDIR}/evb-npcm845_common"

inherit meson pkgconfig

DEPENDS += "libobmc-i2c libgpio-ctrl libkv openssl"

