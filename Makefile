# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026 Vahid Khalafov

CC      := i686-elf-gcc
LD      := i686-elf-gcc
AS      := i686-elf-gcc
CFLAGS  := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -MMD -MP -Iinclude
ASFLAGS := -ffreestanding
LDFLAGS := -T linker.ld -ffreestanding -O2 -nostdlib

SRCDIR  := src
OBJDIR  := build
SRC_C   := $(shell find $(SRCDIR) -name '*.c')
SRC_S   := $(shell find $(SRCDIR) -name '*.s')
OBJS    := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC_C)) \
	$(patsubst $(SRCDIR)/%.s,$(OBJDIR)/%.o,$(SRC_S))
DEPS    := $(OBJS:.o=.d)
KERNEL  := kernel.bin
ISO     := Vahix.iso

.PHONY: all clean iso run

all: $(KERNEL)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.s | $(OBJDIR)
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $@

iso: $(KERNEL)
	cp $(KERNEL) iso/boot/kernel.bin
	grub-mkrescue -o $(ISO) iso

clean:
	rm -rf $(OBJDIR) $(KERNEL) $(ISO) iso/boot/kernel.bin

run: iso
	qemu-system-i386 -audiodev pipewire,id=snd0 -machine pcspk-audiodev=snd0 -enable-kvm -cdrom $(ISO)

-include $(DEPS)
