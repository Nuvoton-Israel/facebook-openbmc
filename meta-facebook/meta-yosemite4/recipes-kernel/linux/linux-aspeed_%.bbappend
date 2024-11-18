FILESEXTRAPATHS:prepend := "${THISDIR}/linux-aspeed:"
SRC_URI += " \
    file://1000-arm-dts-aspeed-yosemite4-enable-Jtag-on-yosemite4.patch \
    file://1001-arm-dts-aspeed-yosemite4-add-mac-config-property.patch \
    file://1002-arm-dts-aspeed-yosemite4-Revise-i2c-duty-cycle.patch \
    file://1003-arm-dts-aspeed-yosemite4-add-power-and-adc-sensors-o.patch \
    file://1004-arm-dts-aspeed-yosemite4-add-multi-source-of-power-s.patch \
    file://1005-arm-dts-aspeed-yosemite4-set-TACH-config-for-MAX3179.patch \
    file://1006-arm-dts-aspeed-yosemite4-add-2nd-source-fan-IC-NCT73.patch \
    file://1007-arm-dts-aspeed-yosemite4-add-EMC1403-for-Terminus-NI.patch \
    file://1008-arm-dts-aspeed-yosemite4-Support-MP5990-for-all-Sent.patch \
    file://1009-ARM-dts-aspeed-yosemite4-add-I3C-config-in-DTS.patch \
    file://1010-ARM-dts-aspeed-yosemite4-add-fan-led-config.patch \
    file://1011-arm-dts-aspeed-yosemite4-add-gpio-related-settings.patch \
    file://1012-ARM-dts-aspeed-yosemite4-modify-I2C-mode-and-freq.patch \
    file://yosemite4-local.cfg \
"
