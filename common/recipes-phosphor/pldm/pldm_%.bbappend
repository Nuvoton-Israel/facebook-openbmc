FILESEXTRAPATHS:prepend:openbmc-fb-lf := "${THISDIR}/files:"

EXTRA_OEMESON:append:openbmc-fb-lf = " -Doem-meta=enabled"

SRC_URI:append:openbmc-fb-lf = " \
    file://host_eid \
		file://0001-platform-mc-PDR-handling.patch \
		file://0002-platform-mc-Sensor-handling.patch \
		file://0003-platform-mc-Added-EventManager.patch \
		file://0004-requester-support-multi-host-MCTP-devices-hot-plug.patch \
		file://0005-Support-OEM-META-write-file-request-for-post-code-hi.patch \
		file://0006-platform-mc-Add-OEM-Meta-event-handler.patch \
		file://0007-Support-OEM-META-command-for-host-BIOS-version.patch \
		file://0008-platform-mc-Monitor-all-sensors-once-upon-the-first-.patch \
		file://0009-pldm-Revise-image-path-for-update.patch \
		file://0010-Support-OEM-META-command-for-Event-Logs-from-BIC.patch \
		file://0011-Support-OEM-META-command-for-power-control.patch \
		file://0012-oem-meta-Add-APML-alert-handler.patch \
		file://0013-Support-OEM-META-command-for-NIC-power-cycle.patch \
    file://pldm-restart.sh \
    file://pldm-slow-restart.service \
"

FILES:${PN}:append:openbmc-fb-lf = " \
    ${systemd_system_unitdir}/pldm-restart.sh \
"

SYSTEMD_SERVICE:${PN}:append:openbmc-fb-lf = " \
    pldm-slow-restart.service \
    "
SYSTEMD_AUTO_ENABLE:${PN}:openbmc-fb-lf = "enable"

do_install:append:openbmc-fb-lf() {
    install -d ${D}/usr/share/pldm
    install -m 0444 ${WORKDIR}/host_eid ${D}/usr/share/pldm
    install -m 0755 ${WORKDIR}/pldm-restart.sh ${D}${datadir}/pldm/
    install -m 0644 ${WORKDIR}/pldm-slow-restart.service ${D}${systemd_system_unitdir}
}

