## Installation

You need to install the cross-compiling tools with `make tools`.

You can then `make kernel` to make the kernel, or `make iso` to make a bootable ISO. You can also `make boot` and `make multiboot` to boot these in QEMU, respectively.

## Debugging
`make debug`

## Dependencies
Basic dependencies:
- `nasm`
- `gcc`
- `make`
- `qemu`

To create a bootable ISO:
- `mtools`
- `libburniso`

To debug:
- `gdb`
