FILESEXTRAPATHS:append := "${THISDIR}/files:"

SUMMARY = "Firmware version populators"
DESCRIPTION = "Scripts to retrieve firmware versions and update settingsd"

PR = "r1"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/files/common-licenses/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

S = "${WORKDIR}"

FCB_INSTANCES="1:2:3:4:5:6"

FW_TOOLS = "\
    scm-bmc,minerva-sys-init.service,multi-user.target,multi-user.target,0 \
    bmc-tpm,minerva-sys-init.service,multi-user.target,multi-user.target,0 \
    scm-cpld,minerva-sys-init.service,multi-user.target,multi-user.target,0 \
    cmm-cpld,minerva-sys-init.service,multi-user.target,multi-user.target,0 \
    fcb-cpld,minerva-sys-init.service,multi-user.target,multi-user.target,${FCB_INSTANCES} \
    ethernet-switch,minerva-sys-init.service,multi-user.target,multi-user.target,0 \
"

LOCAL_URI = " \
    file://fw-versions@.service \
    ${@ ' '.join([ f"file://" + x.split(',')[0] for x in d.getVar('FW_TOOLS', True).split() ])} \
"

RDEPENDS:${PN}:append = " \
    bash \
    cpld-fw-handler \
    fw-util \
"

FILES:${PN}:append = " \
    ${systemd_system_unitdir} \
"

do_install:append() {
    install -d ${D}${systemd_system_unitdir}
    install -d ${D}${libexecdir}/${BPN}

    for f in ${FW_TOOLS}; do
        SERVICE=$(echo "$f" | cut -d, -f1)
        AFTER=$(echo "$f" | cut -d, -f2 | sed 's/_/ /g')
        BEFORE=$(echo "$f" | cut -d, -f3 | sed 's/_/ /g')
        WANTEDBY=$(echo "$f" | cut -d, -f4 | sed 's/_/ /g')

        install -m 0755 ${S}/$SERVICE ${D}${libexecdir}/${BPN}/$SERVICE

        install -m 0644 ${S}/fw-versions@.service \
            ${D}${systemd_system_unitdir}/fw-versions-$SERVICE@.service

        sed -i ${D}${systemd_system_unitdir}/fw-versions-$SERVICE@.service \
            -e "s/{SERVICE}/$SERVICE/g" \
            -e "s/{AFTER}/$AFTER/g" \
            -e "s/{BEFORE}/$BEFORE/g" \
            -e "s/{WANTEDBY}/$WANTEDBY/g"
    done
}

def fw_version_services(d):
    services = []

    for f in d.getVar('FW_TOOLS', True).split():
        (service, afters, befores, wantedbys, instances) = f.split(',')

        for i in instances.split(":"):
            for s in wantedbys.split('_'):
                wantedby_str = s.replace("%i", str(i))

                services.append(
                    f"{wantedby_str}:fw-versions-{service}@{i}.service:fw-versions-{service}@.service"
                )

    return " ".join(services)

pkg_postinst:${PN}:append() {
    for s in ${@fw_version_services(d)}; do
        TARGET=$(echo "$s" | cut -d: -f1)
        INSTANCE=$(echo "$s" | cut -d: -f2)
        SERVICE=$(echo "$s" | cut -d: -f3)

        mkdir -p "$D${systemd_system_unitdir}/$TARGET.wants"
        ln -s "../$SERVICE" "$D${systemd_system_unitdir}/$TARGET.wants/$INSTANCE"
    done
}

pkg_prerm:${PN}:append() {
    for s in ${@fw_version_services(d)}; do
        TARGET=$(echo "$s" | cut -d: -f1)
        INSTANCE=$(echo "$s" | cut -d: -f2)

        rm "$D${systemd_system_unitdir}/$TARGET.wants/$INSTANCE"
    done
}
