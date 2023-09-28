#!/bin/bash

set -e

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PREFIX="$DIR/tools/"
TARGET="i686-elf"
PATH="$PREFIX/bin:$PATH"
BUILD="$DIR/../build"
NUM_JOBS=$(( $(nproc) / 2 ))

GNU_MIRROR="https://mirrors.ocf.berkeley.edu/gnu"

BINUTILS_VERSION="2.41"
BINUTILS_FILE="binutils-$BINUTILS_VERSION"
BINUTILS_URL="$GNU_MIRROR/binutils/$BINUTILS_FILE.tar.gz"

GCC_VERSION="13.2.0"
GCC_FILE="gcc-$GCC_VERSION"
GCC_URL="$GNU_MIRROR/gcc/gcc-$GCC_VERSION/$GCC_FILE.tar.gz"

source "$DIR/../scripts/log.sh"

download_binutils() {
	if [ ! -d "$BUILD/$BINUTILS_FILE" ]; then
		msg "Downloading binutils $BINUTILS_VERSION..."
		curl "$BINUTILS_URL" > "$DIR/$BINUTILS_FILE.tar.gz" || exit 1
		msg "Extracting binutils..."
		tar -xzf "$DIR/$BINUTILS_FILE.tar.gz" || exit 1
		rm "$DIR/$BINUTILS_FILE.tar.gz"
	else
		msg "Already downloaded binutils."
	fi
}


download_gcc() {
	if [ ! -d "$BUILD/$GCC_FILE" ]; then
		msg "Downloading gcc $GCC_VERSION..."
		curl "$GCC_URL" > "$DIR/$GCC_FILE.tar.gz" || exit 1
		msg "Extracting gcc..."
		tar -xzf "$DIR/$GCC_FILE.tar.gz" || exit 1
		rm "$DIR/$GCC_FILE.tar.gz"
	else
		msg "Already downloaded gcc."
	fi
}


build_binutils() {
	pushd .
	cd "$BUILD"

	msg "Configuring binutils..."
	mkdir -p "binutils-$BINUTILS_VERSION-build"
	cd "binutils-$BINUTILS_VERSION-build"
	"$BUILD/$BINUTILS_FILE/configure" --target="$TARGET" --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror || exit 1
	
	msg "Making binutils..."
	make -j "$NUM_JOBS" || exit 1
	
	msg "Installing binutils..."
	make install || exit 1
	
	success "Installed binutils!"
	cd ..
	popd
}


build_gcc() {
  pushd .
  cd "$BUILD"

  msg "Configuring gcc..."
  mkdir -p "gcc-$GCC_VERSION-build"
  cd "gcc-$GCC_VERSION-build"
  "$BUILD/$GCC_FILE/configure" --prefix="$PREFIX" --target="$TARGET" --disable-nls --enable-languages=c --without-headers || exit 1

  msg "Making gcc..."
  make -j "$NUM_JOBS" all-gcc all-target-libgcc || exit 1
  msg "Installing gcc..."
  make install-gcc install-target-libgcc || exit 1
  success "Installed gcc!"

  cd ..
  popd
}

mkdir -p "$BUILD" && cd $BUILD

download_binutils
download_gcc

msg "Building binutils and gcc..."
build_binutils
build_gcc

success "Successfully built the Tomos toolchain!"
