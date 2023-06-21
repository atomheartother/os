## Installation

You need to install the cross-compiling tools with `make tools`.

You can then `make kernel` to make the kernel, or `make iso` to make a bootable ISO. You can also `make boot` and `make multiboot` to boot these in QEMU, respectively.

## Debugging
`make debug`

## Dependencies
Basic dependencies:
- `nasm` (To compile assembly)
- `gcc` (To compile C)
- `make` (To make!)

To emulate booting:
- `qemu` 

To create a bootable ISO:
- `mtools`
- `libburniso`

To debug:
- `gdb`
