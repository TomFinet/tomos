# Tomos: bare metal x86 memory management 

A x86 memory manager which has:

- [x] page-frame allocator.
- [x] slab allocator.
- [x] unit tests.

## How to run it.

### Build the toolchain.

Run `./toolchain/build_toolchain.sh`.

### Build and run the kernel with QEMU

Run the `clean_install.sh` script. Just runs image on qemu, unexciting at the moment.

### Run the unit tests

Run the `build_tests.sh` script. Executes every test suite fully.