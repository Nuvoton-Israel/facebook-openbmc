FILESEXTRAPATHS:prepend := "${THISDIR}/linux-nuvoton:"

SRC_URI:append:evb-npcm845 = " file://evb-npcm845.cfg"
#SRC_URI:append:evb-npcm845 = " file://0001-dts-npcm8xx-add-psci-smp-method-tz.patch"
#SRC_URI:append:evb-npcm845 = " file://0002-device-tree-optee-enable.patch"
#SRC_URI:append:evb-npcm845 = " file://0003-dts-nuvoton-evb-npcm845-remove-video.patch"

SRC_URI:append:evb-npcm845 = " file://0001-update-i3c-driver.patch"
SRC_URI:append:evb-npcm845 = " file://0001-dts-nuvoton-npcm845-5.15-remove-video.patch"
SRC_URI:append:evb-npcm845 = " file://0001-dts-npcm845-5.15-enable-i3c0.patch"