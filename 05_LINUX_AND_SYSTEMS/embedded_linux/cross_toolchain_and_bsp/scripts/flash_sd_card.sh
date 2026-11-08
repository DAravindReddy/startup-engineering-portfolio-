#!/usr/bin/env bash
# ==============================================================================
# Kernelwise Labs - Target SD Card Partitioning & Flashing Utility
# ==============================================================================
set -euo pipefail

if [ "$#" -ne 2 ]; then
    echo "Usage: sudo $0 <TARGET_DEVICE (e.g., /dev/sdX)> <DEPLOY_DIR>"
    exit 1
fi

DEVICE="$1"
DEPLOY_DIR="$2"

if [ ! -b "${DEVICE}" ]; then
    echo "[ERROR] Device ${DEVICE} does not exist or is not a block device."
    exit 1
fi

echo "[CAUTION] This will format ${DEVICE} and wipe all existing data."
read -p "Type 'YES' to proceed: " CONFIRM
if [ "${CONFIRM}" != "YES" ]; then
    echo "Aborted."
    exit 0
fi

echo "[INFO] Zeroing partition table on ${DEVICE}..."
dd if=/dev/zero of="${DEVICE}" bs=1M count=10 status=progress
sync

echo "[INFO] Partitioning device..."
# 64MB FAT32 Boot (Type c), remainder ext4 Rootfs (Type 83)
parted -s "${DEVICE}" mklabel msdos
parted -s "${DEVICE}" mkpart primary fat32 1MiB 65MiB
parted -s "${DEVICE}" set 1 boot on
parted -s "${DEVICE}" mkpart primary ext4 65MiB 100%

echo "[INFO] Formatting partitions..."
mkfs.vfat -F 32 -n "BOOT" "${DEVICE}1"
mkfs.ext4 -F -L "ROOTFS" "${DEVICE}2"

echo "[SUCCESS] SD Card partitioned and formatted successfully."
