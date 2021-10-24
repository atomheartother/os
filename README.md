## Installation

You need to install the cross-compiling tools with `make tools`.

- To make and boot the kernel directly: `make boot`
- To make and boot an ISO with GRUB support: `make multiboot`

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
