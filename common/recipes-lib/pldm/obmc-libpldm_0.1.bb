# Copyright 2018-present Facebook. All Rights Reserved.
SUMMARY = "PLDM Library"
DESCRIPTION = "library to provide PLDM functionality"
SECTION = "base"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://pldm.c;beginline=5;endline=17;md5=da35978751a9d71b73679307c4d296ec"

S="${WORKDIR}/sources"
UNPACKDIR="${S}"

LOCAL_URI = " \
    file://meson.build \
    file://pldm.c \
    file://pldm.h \
    file://pldm_base.h \
    file://pldm_fw_update.h \
    file://pldm_pmc.h \
    file://pldm_pmc.c \
    "

DEPENDS =+ "libncsi"
RDEPENDS:${PN} =+ "libncsi"

inherit meson pkgconfig
