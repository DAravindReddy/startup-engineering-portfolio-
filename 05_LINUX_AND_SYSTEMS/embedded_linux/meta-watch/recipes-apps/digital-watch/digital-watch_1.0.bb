SUMMARY = "Embedded Linux Multi-Threaded Digital Watch Application"
DESCRIPTION = "Production C11 digital watch timekeeping, stopwatch, countdown timer, and alarm engine developed by Kernelwise Labs."
HOMEPAGE = "https://github.com/DAravindReddy/startup-engineering-portfolio"
SECTION = "apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://digital_watch_system/ \
           file://digital-watch.service"

S = "${WORKDIR}/digital_watch_system"

inherit systemd

SYSTEMD_SERVICE:${PN} = "digital-watch.service"
SYSTEMD_AUTO_ENABLE = "enable"

TARGET_CFLAGS += "-O2 -Wall -Wextra"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} -I${S}/include \
        ${S}/src/watch_core.c \
        ${S}/src/display.c \
        ${S}/src/main.c \
        -o digital_watch
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 digital_watch ${D}${bindir}/digital_watch

    if ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'true', 'false', d)}; then
        install -d ${D}${systemd_unitdir}/system
        install -m 0644 ${WORKDIR}/digital-watch.service ${D}${systemd_unitdir}/system/
    fi
}

FILES:${PN} += "${bindir}/digital_watch ${systemd_unitdir}/system/digital-watch.service"
