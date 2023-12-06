FILESEXTRAPATHS:prepend := "${THISDIR}/files/pal:"

LOCAL_URI += "\
    file://pal_health.c \
    file://pal_health.h \
    file://pal_power.c \
    file://pal_power.h \
    file://pal_cfg.c \
    file://pal_cfg.h \
    file://pal_common.c \
    file://pal_common.h \
    file://pal_def.h \
    file://pal_def_cover.h \
    file://pal_sensors.c \
    file://pal_sensors.h \
    file://pal_mb_sensors.c \
    file://pal_mb_sensors.h \
    file://pal_swb_sensors.c \
    file://pal_swb_sensors.h \
    file://pal_hgx_sensors.c \
    file://pal_hgx_sensors.h \
    file://pal_ubb_sensors.c \
    file://pal_bb_sensors.c \
    file://pal_bb_sensors.h \
    file://pal_cover.c \
    file://pal_acb_sensors.c \
    file://pal_acb_sensors.h \
    file://pal_meb_sensors.c \
    file://pal_meb_sensors.h \
    file://plat/meson.build \
    "

DEPENDS += " \
    libhgx \
    libhal \
    libgpio-ctrl \
    libmisc-utils \
    libncsi \
    libnl-wrapper \
    libnodemanager \
    libobmc-i2c \
    libobmc-pmbus \
    libobmc-sensors \
    libpeci \
    libpeci-sensors \
    libpldm-oem \
    libsensor-correction \
    libdimm \
    libretimer \
    libobmc-i3c \
    apml \
    "

# These shouldn't be needed but are because we aren't properly versioning the
# shared libraries contained in these recipes.
RDEPENDS:${PN} += " \
    libpeci \
    libsensor-correction \
    apml \
    "
