#!/bin/bash

# create a raw image of the correct size
# setup a loopback device for the image
# create a partition table and filesystem
# mount the filesystem
# install grub on the image

set -e

SCRIPTDIR=$(dirname "$BASH_SOURCE")
source "$SCRIPTDIR/log.sh"

TARGET=i386-pc

IMAGE_INSTALL_PATH="img/boot/grub"

IMAGE_NAME="tomos.img"
IMAGE_EXTRASIZE="100000"

if [ "$(id -u)" != 0 ]; then
  exec sudo -E -- "$0" "$@" || fail "Please run as root"
fi

if [ $# -eq 0 ]; then
  # Calculate size of image
  USER_SIZE=0
  if [ -d "${SOURCE_DIR}/user" ]; then
    USER_SIZE=$(du -sk "${SOURCE_DIR}/user" | cut -f1)
  fi
  IMAGE_SIZE=$(($(du -sk root | cut -f1) + IMAGE_EXTRASIZE + USER_SIZE))
  IMAGE_SIZE=$(expr '(' '(' "$IMAGE_SIZE" + 1023 ')' / 1024 ')' '*' 1024)

  if [ -f "$IMAGE_NAME" ]; then
    USE_EXISTING=1
    msg "Using existing image..."
  else
    msg "Creating image ($IMAGE_SIZE K)..."
    qemu-img create -q -f raw "$IMAGE_NAME" "$IMAGE_SIZE"k || fail "Couldn't create image"
    chown "$SUDO_UID":"$SUDO_GID" "$IMAGE_NAME"
  fi

	msg "Making loopback device..."
	dev=$(losetup --find --partscan --show "$IMAGE_NAME")
	part="p1"
	if [ -z "$dev" ]; then
		fail "Couldn't create loopback device."
	fi
	msg "Loopback device mounted at ${dev}"
else
  dev="$1"
  part="1"
  msg "Using $1..."
fi

if [ ! "$USE_EXISTING" ]; then
  msg "Creating partition table..."
  parted -s "${dev}" mklabel msdos mkpart primary ext2 32k 100% -a minimal set 1 boot on || fail "Couldn't partition image."

  msg "Creating ext2 filesystem..."
  yes | mke2fs -q -I 128 -b 1024 "${dev}${part}" || fail "Couldn't create filesystem."
fi

msg "Mounting filesystem on ${dev}${part} to mnt ..."
mkdir -p mnt/
mount "${dev}${part}" mnt/ || fail "Couldn't mount."
MOUNTED="1"

if [ ! "$USE_EXISTING" ]; then
  grub-install --target=$TARGET --boot-directory=mnt/boot --modules="ext2 part_msdos" "${dev}" $GRUB_EXTRAARGS || fail "Couldn't install grub."
  if [[ -d "mnt/boot/grub" ]]; then
      cp "${SOURCE_DIR}/scripts/grub.cfg" mnt/boot/grub/grub.cfg || fail "Couldn't copy grub.cfg."
  else
      fail "Couldn't find GRUB installation directory."
  fi
fi

detach_and_unmount() {
  msg "Cleaning up..."
  if [ ! -z "$MOUNTED" ]; then
    umount mnt || fail "Couldn't unmount."
    rmdir mnt || fail "Couldn't rmdir mnt."
  fi

  if [ ! -z "$dev" ]; then
    losetup -d "${dev}" || fail "Couldn't clean up loopback device."
  fi
}

msg "Setting up base filesystem..."
bash "${SCRIPTDIR}/base-system.sh" mnt/ || fail "Couldn't make base filesystem."

if [ $# -eq 0 ]; then
  detach_and_unmount
  success "Done! Saved to $IMAGE_NAME."
else
  success "Done! Image created on $1."
fi