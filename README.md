# Tomos: the OS made for Tom's by Tom's

A pedagogical operating system for the x86 architecture.

## How to run it.

### Build the toolchain.

Run `./toolchain/build_toolchain.sh`.

### Configure CMake.

Run `mkdir -p build` and then `cd build && cmake .. -DCMAKE_TOOLCHAIN_FILE=toolchain/CMakeToolchain.txt`.

### Build the kernel.

From `build/`, run `make install`.

### Create the bootable kernel image.

Run `make image`.

### Run on qemu.

Run `make qemu`.

## Directory structure.

- *kernel*: the main kernel source.
- *include*: the kernel header files.
- *toolchain*: scripts for building the cross-compiling toolchain for tomos.
- *scripts*: scripts for making a bootable image and running the image on qemu.
