CC      := i686-elf-gcc
LD      := i686-elf-gcc
AS      := i686-elf-gcc
CFLAGS  := -std=gnu99 -ffreestanding -O2 -Wall -Wextra
ASFLAGS := -ffreestanding
LDFLAGS := -T linker.ld -ffreestanding -O2 -nostdlib

OBJDIR  := build
OBJS    := $(OBJDIR)/boot.o $(OBJDIR)/kernel.o
KERNEL  := kernel.bin
ISO     := VahidOS.iso

.PHONY: all clean run iso

all: $(KERNEL)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/boot.o: src/boot.s | $(OBJDIR)
	$(AS) $(ASFLAGS) -c $< -o $@

$(OBJDIR)/kernel.o: src/kernel.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) $(OBJS) -o $@

iso: $(KERNEL)
	cp $(KERNEL) iso/boot/kernel.bin
	grub-mkrescue -o $(ISO) iso

run: iso
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(OBJDIR) $(KERNEL) $(ISO) iso/boot/kernel.bin
