# Copyright 2024-present Facebook. All Rights Reserved.

SUMMARY = "HMC Utility Tool"
PR = "r1"
LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/GPL-2.0-only;md5=801f80980d171dd6425610833a22dbe6"

inherit systemd
inherit obmc-phosphor-systemd

S="${WORKDIR}/sources"
UNPACKDIR="${S}"

LOCAL_URI += " \
    file://hmc-util \
    "

RDEPENDS:${PN} += " bash"

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = " \
    sync-hmc-datetime.service \
    sync-hmc-datetime.timer \
    "

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${UNPACKDIR}/hmc-util ${D}${bindir}/hmc-util
}
