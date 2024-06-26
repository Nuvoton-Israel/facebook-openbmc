FILESEXTRAPATHS:prepend := "${THISDIR}/linux-nuvoton:${THISDIR}/../../../recipes-kernel/linux/linux-aspeed:"

KBRANCH = "dev-6.6"
LINUX_VERSION = "6.6.39"

SRCREV="ee1cd5048959de496cd005c50b137212a5b62062"

SRC_URI:append = " file://yosemite4n.cfg"

# Common driver
SRC_URI:append = " file://0501-hwmon-ina233-Add-ina233-driver.patch"
SRC_URI:append = " file://0503-Add-adm1281-driver.patch"
SRC_URI:append = " file://0510-Add-ina233-and-ina238-devicetree-config-for-EVT-sche.patch"
SRC_URI:append = " file://0515-hwmon-Driver-for-Nuvoton-NCT7363Y.patch"
SRC_URI:append = " file://0519-i3c-master-add-enable-disable-hot-join-in-sys-entry.patch"
SRC_URI:append = " file://0523-dt-bindings-i3c-hub-Add-Renesas-RG3MxxB12A1-I3C-HUB.patch"
SRC_URI:append = " file://0524-i3c-i3c-hub-Add-Renesas-RG3MxxB12A1-I3C-HUB-driver.patch"

# NPCM8XX common dts
SRC_URI:append = " file://1000-arm64-dts-adding-NPCM8xx-modules-node.patch"

# NPCM8XX YV4 dts
SRC_URI:append = " file://1015-arm64-dts-nuvoton-add-initial-yosemitev4-device-tree.patch"

# NPCM8XX clock driver
SRC_URI:append = " file://1100-clk-npcm8xx-add-clock-controller.patch"

# NPCM8XX NCSI driver
SRC_URI:append = " file://1300-net-stmmac-Add-NCSI-support-for-STMMAC.patch"

# NPCM8XX pinctrl driver
SRC_URI:append = " file://1401-pinctrl-npcm8xx-Add-GPIO250-pin-config.patch"
SRC_URI:append = " file://1402-pinctrl-npcm8xx-clear-polarity-before-setting-interr.patch"
SRC_URI:append = " file://1403-pinctrl-npcm8xx-add-GPI35-and-GPI36.patch"
SRC_URI:append = " file://1405-pinctrl-npcm8xx-correct-gpio162-and-gpio168-setting.patch"
SRC_URI:append = " file://1406-pinctrl-npcm8xx-Correct-sg1mdio-smi-pspi-and-remove-.patch"
SRC_URI:append = " file://1407-pinctrl-npcm8xx-update-pins-slew-rate-and-DS.patch"

# NPCM8XX FIU driver
SRC_URI:append = " file://1500-spi-npcm-fiu-add-dual-and-quad-write-support.patch"
SRC_URI:append = " file://1501-spi-npcm-fiu-add-delay-before-reboot-to-avoi.patch"

# NPCM8XX PSPI driver
SRC_URI:append = " file://1600-spi-npcm-pspi-Add-full-duplex-support.patch"
SRC_URI:append = " file://1601-spi-npcm-pspi-Fix-transfer-bits-per-word-issue-389.patch"

# NPCM8XX UDC driver
SRC_URI:append = " file://1700-usb-chipidea-udc-enforce-write-to-the-memory.patch"

# NPCM8XX Jtag Master driver
SRC_URI:append = " file://1800-misc-npcm8xx-jtag-master-Add-NPCM845-JTAG-master-dri.patch"
SRC_URI:append = " file://1804-dt-binding-bmc-npcm8xx-Add-NPCM845-JTAG-master-docum.patch"

# NPCM8XX i2c driver
SRC_URI:append = " file://1900-i2c-npcm-disable-interrupt-enable-bit-before-devm_re.patch"

# NPCM8XX ADC driver (For YV4 only)
SRC_URI:append = " file://2000-adc-npcm-increase-timeout-to-500ms.patch"

# NPCM8XX i3c driver (Work in procress)
SRC_URI:append = " file://2100-i3c-expose-interface-for-doing-daa-request.patch"
SRC_URI:append = " file://1014-arm64-dts-npcm8xx-modify-i3c-node-to-support-npcm8xx.patch"
SRC_URI:append = " file://3001-dt-bindings-i3c-Add-NPCM845-i3c-controller.patch"
SRC_URI:append = " file://3002-i3c-master-Add-Nuvoton-npcm845-i3c-master-driver.patch"
