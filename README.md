# Vahix

<!-- SPDX-License-Identifier: GPL-2.0-only -->

![Example](docs/image.png)

## Vahix is an experimental kernel.

## Layout
- `src/arch/i386` — boot code and architecture-specific startup
- `src/kernel` — kernel entry point and shell
- `src/drivers` — device helpers (keyboard, VGA text)
- `src/lib` — freestanding libc-style utilities
- `include` — public headers mirrored to the source layout
- `iso/boot` — GRUB config and the staged `kernel.bin` for ISO creation
- `docs` — static site assets
- `build` — generated object files (gitignored)

## Build

Requirements:
- `i686-elf-gcc` cross compiler
- `grub-mkrescue`

Commands:
- `make` builds `kernel.bin`
- `make iso` creates `Vahix.iso`
- `make clean` cleans binaries

## Advice

If you're running QEMU, and want to have sound output:
`qemu-system-x86_64 -audiodev pipewire,id=snd0 -machine pcspk-audiodev=snd0`
These are the recommended flags if you are using pipewire with the `pcspk` driver, which is the only one currently supported. If you are using pulseaudio, you can replace `pipewire` with `pa` and it should work as well.

## License

Vahix is licensed under GPLv2 only. See `LICENSE` for the full text.
