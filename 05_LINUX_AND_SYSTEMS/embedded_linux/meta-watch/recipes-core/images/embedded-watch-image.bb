SUMMARY = "Minimal Embedded Linux Watch Image"
DESCRIPTION = "Custom lean Linux image for IoT edge nodes featuring Kernelwise digital-watch and diagnostics utilities."
LICENSE = "MIT"

inherit core-image

# Image feature definitions
IMAGE_FEATURES += "ssh-server-dropbear hwcodecs splash"

# Required packages
IMAGE_INSTALL += "\
    packagegroup-core-boot \
    digital-watch \
    i2c-tools \
    spitools \
    can-utils \
    iproute2 \
    htop \
    tzdata \
"

# Set root partition size (512 MB target)
IMAGE_ROOTFS_SIZE ?= "524288"
IMAGE_ROOTFS_EXTRA_SPACE ?= "65536"
