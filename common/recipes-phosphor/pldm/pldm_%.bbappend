FILESEXTRAPATHS:prepend:openbmc-fb-lf := "${THISDIR}/files:"

# EXTRA_OEMESON:append:openbmc-fb-lf = " -Doem-meta=enabled"

SRC_URI:append:openbmc-fb-lf = " \
    file://host_eid \
    file://0001-requester-support-multi-host-MCTP-devices-hot-plug.patch \
    file://0002-pldm-Revise-image-path-for-update.patch \
    file://0003-Support-OEM-META-write-file-request-for-post-code-hi.patch \
    file://0004-platform-mc-Add-OEM-Meta-event-handler.patch \
    file://0005-Support-OEM-META-command-for-host-BIOS-version.patch \
    file://0006-Support-OEM-META-command-for-Event-Logs-from-BIC.patch \
    file://0007-Support-OEM-META-command-for-power-control.patch \
    file://0008-oem-meta-Add-APML-alert-handler.patch \
    file://0009-Support-OEM-META-command-for-NIC-power-cycle.patch \
    file://0010-Add-event-log-type-for-PMIC-error-VR-alert.patch \
    file://0011-Update-retimer-version-after-post-complete.patch \
    file://0012-Support-OEM-META-command-for-getting-Http-boot-certi.patch \
    file://0013-platform-mc-Handle-get-PLDM-Commands-failure.patch \
"

SYSTEMD_AUTO_ENABLE:${PN}:openbmc-fb-lf = "enable"

do_install:append:openbmc-fb-lf() {
    install -d ${D}/usr/share/pldm
    install -m 0444 ${WORKDIR}/host_eid ${D}/usr/share/pldm
}

