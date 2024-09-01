# Tomos: x86 system programming environment

The goal of Tomos is to provide a simple programming environment somewhere in between bare-metal and user-space, for people who want to learn about system programming (my motivation for Tomos in the first place). Users of Tomos are badass and write code with kernel priveldges without the need for silly system calls. If that sounds like you, epic!

Currently, it supports:

- [x] interrupt handling.
- [x] page-frame allocator.
- [x] slab allocator.
- [ ] *kernel threads coming soon...* 

## How to run it.

### Build the toolchain.

Run `./toolchain/build_toolchain.sh`.

### Build and run the kernel with QEMU

Run the `clean_install.sh` script, which runs the Tomos image on qemu.

### Run the unit tests

Run the `build_tests.sh` script, which executes all unit tests.
