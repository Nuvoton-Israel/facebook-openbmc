# Copyright 2015-present Facebook. All Rights Reserved.
SUMMARY = "PSB Utility"
DESCRIPTION = "Util for reading PSB config"
SECTION = "base"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

inherit meson pkgconfig
inherit legacy-packages

S="${WORKDIR}/sources"
UNPACKDIR="${S}"

LOCAL_URI = " \
    file://psb-util.cpp \
    file://psb-platform.cpp \
    file://meson.build \
    "

pkgdir = "psb-util"

DEPENDS = " libbic libpal libkv nlohmann-json cli11"
RDEPENDS:${PN} += " libbic libpal libkv"
