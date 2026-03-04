# VahidOS

VahidOS is an experimental kernel.

## Build

Requirements:
- `i686-elf-gcc` cross compiler
- `grub-mkrescue`
- `qemu-system-i386` (for running)

Commands:
- `make` builds `kernel.bin`
- `make iso` creates `VahidOS.iso`
- `make run` boots the ISO in QEMU
