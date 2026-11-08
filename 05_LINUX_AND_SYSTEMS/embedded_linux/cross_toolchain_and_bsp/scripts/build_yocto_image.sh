#!/usr/bin/env bash
# ==============================================================================
# Kernelwise Labs - Yocto BitBake Image Build Automation
# ==============================================================================
set -euo pipefail

YOCTO_DIR="${HOME}/poky-kirkstone"
BUILD_DIR="${YOCTO_DIR}/build"
LAYER_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../../meta-watch" && pwd)"

echo "====================================================================="
echo " Kernelwise Labs: Yocto Image Generation Engine                      "
echo " Target Image: embedded-watch-image                                  "
echo "====================================================================="

if [ ! -d "${YOCTO_DIR}" ]; then
    echo "[INFO] Poky repository not found. Cloning poky (kirkstone branch)..."
    git clone -b kirkstone git://git.yoctoproject.org/poky "${YOCTO_DIR}"
fi

cd "${YOCTO_DIR}"
source oe-init-build-env "${BUILD_DIR}"

# Append meta-watch layer if not present
if ! bitbake-layers show-layers | grep -q "meta-watch"; then
    echo "[INFO] Adding meta-watch layer to bblayers.conf..."
    bitbake-layers add-layer "${LAYER_DIR}"
fi

echo "[INFO] Commencing BitBake image build (embedded-watch-image)..."
bitbake embedded-watch-image

echo "[SUCCESS] Build completed successfully."
echo "Deploy directory: ${BUILD_DIR}/tmp/deploy/images/"
