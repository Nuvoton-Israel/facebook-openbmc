require recipes-core/images/yosemite4-image.bb

IMAGE_INSTALL:remove = " addc"
IMAGE_INSTALL:remove = " yaap"
IMAGE_INSTALL:remove = " packagegroup-openbmc-tests2"
