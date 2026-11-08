#!/usr/bin/env bash
# ==============================================================================
# Kernelwise Labs - Embedded Linux Cross-Compilation Environment Setup
# ==============================================================================
set -euo pipefail

echo "====================================================================="
echo " Configuring Embedded Linux ARM/ARM64 Cross-Compilation Toolchain   "
echo "====================================================================="

TARGET_ARCH="${1:-armhf}"
SDK_INSTALL_DIR="/opt/kernelwise-toolchain"

if [ "${TARGET_ARCH}" = "armhf" ]; then
    export CROSS_COMPILE=arm-linux-gnueabihf-
    export ARCH=arm
    echo "[INFO] Configured 32-bit ARM Hard-Float (arm-linux-gnueabihf)"
elif [ "${TARGET_ARCH}" = "aarch64" ]; then
    export CROSS_COMPILE=aarch64-linux-gnu-
    export ARCH=arm64
    echo "[INFO] Configured 64-bit ARM AArch64 (aarch64-linux-gnu)"
else
    echo "[ERROR] Unsupported architecture: ${TARGET_ARCH}"
    echo "Usage: source setup_cross_toolchain.sh [armhf|aarch64]"
    return 1 2>/dev/null || exit 1
fi

export CC="${CROSS_COMPILE}gcc"
export CXX="${CROSS_COMPILE}g++"
export LD="${CROSS_COMPILE}ld"
export AR="${CROSS_COMPILE}ar"
export STRIP="${CROSS_COMPILE}strip"
export CFLAGS="-O2 -pipe -Wall -Wextra"
export CXXFLAGS="-O2 -pipe -Wall -Wextra"

echo "[SUCCESS] Cross-compilation variables exported:"
echo "  CC            = ${CC}"
echo "  ARCH          = ${ARCH}"
echo "  CROSS_COMPILE = ${CROSS_COMPILE}"
