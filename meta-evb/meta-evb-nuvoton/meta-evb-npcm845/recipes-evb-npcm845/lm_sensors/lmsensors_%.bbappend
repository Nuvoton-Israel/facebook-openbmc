
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://evb-npcm845.conf \
           "

do_install:append() {
    install -d ${D}${sysconfdir}/sensors.d
    install -m 644 ../evb-npcm845.conf ${D}${sysconfdir}/sensors.d/evb-npcm845.conf
}
