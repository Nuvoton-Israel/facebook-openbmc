FILESEXTRAPATHS:append := "${THISDIR}/files:"

SUMMARY = "OpenBMC Inject PMIC error Tool"
DESCRIPTION = "OpenBMC Inject PMIC error Tool"

PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

S = "${WORKDIR}/sources"
UNPACKDIR = "${S}"

LOCAL_URI = " \
    file://pmic-error-injection.sh \
    "

RDEPENDS:${PN} = "bash"

do_install() {
    install -d ${D}/${bindir}
    install -m 0755 ${UNPACKDIR}/pmic-error-injection.sh ${D}/${bindir}/
}
