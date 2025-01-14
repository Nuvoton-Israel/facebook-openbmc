DESCRIPTION="Infineon TPM firmware updater"
SECTION = "base"
PR = "r1"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=429c5ef91a429a84d98af1c29d26216c"

S="${WORKDIR}/sources"
UNPACKDIR="${S}"

LOCAL_URI = "file://Source/TPMFactoryUpd \
             file://Source/Common \
             file://LICENSE \
             file://00-modify_makefile.patch \
            "

DEPENDS += "openssl"

INSANE_SKIP:${PN}:append = "already-stripped"

binfiles = "TPMFactoryUpd"

do_compile() {
  oe_runmake -C ${UNPACKDIR}/Source/TPMFactoryUpd
}

do_install() {
  bin="${D}/usr/local/bin"
  install -d $bin
  install -m 755 ${UNPACKDIR}/Source/TPMFactoryUpd/${binfiles} ${bin}/${binfiles}
}

FILES:${PN} = "${prefix}/local/bin"
