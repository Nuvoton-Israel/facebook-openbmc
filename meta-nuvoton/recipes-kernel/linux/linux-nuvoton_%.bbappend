
FILESEXTRAPATHS:prepend := "${THISDIR}/files/${SOC_FAMILY}:"

SRC_URI = "git://github.com/Nuvoton-Israel/linux;protocol=https;branch=${KBRANCH}"
SRC_URI += "file://defconfig \
           "

KBRANCH = "NPCM-5.15-OpenBMC"
LINUX_VERSION = "5.15.50"
SRCREV = "9060e414cda02d2e8c13390ebaba119a9c533f36"

LINUX_VERSION_EXTENSION = "-nuvoton"

