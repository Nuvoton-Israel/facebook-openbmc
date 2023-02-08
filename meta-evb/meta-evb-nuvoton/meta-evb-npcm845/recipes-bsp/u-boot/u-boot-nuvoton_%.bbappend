FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRCREV = "da7f635f0d425843cd458ba4a590317efb531adc"

SRC_URI += "file://0001-arble-configs-Add-fdt_high-and-initrd_high-variables.patch\
            file://0002-uboot-dts-refine-flash-spi-max-frequency.patch \
           "
