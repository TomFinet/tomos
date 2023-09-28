# Tomos: easy to use x86 memory management kernel 

A x86 memory manager which has:

- [x] page-frame allocator.
- [x] SLAB allocator.
- [x] unit tests.

## How to run it.

### Build the toolchain.

Run `./toolchain/build_toolchain.sh`.

### Configure CMake.

Run `mkdir -p build` and then `cd build && cmake .. -DCMAKE_TOOLCHAIN_FILE=toolchain/CMakeToolchain.txt`.

### Build and run the kernel with QEMU

Run the `clean_install.sh` script.

### Run the unit tests

Run the `build_tests.sh` script.

## Directory structure.

- *kernel*: the main kernel source.
- *include*: the kernel header files.
- *toolchain*: scripts for building the cross-compiling toolchain for tomos.
- *scripts*: scripts for making a bootable image and running the image on qemu.
