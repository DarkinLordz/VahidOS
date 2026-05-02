CC      := gcc
LD      := gcc
AS      := gcc
CFLAGS  := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -MMD -MP -Iinclude -m32
ASFLAGS := -ffreestanding -m32
LDFLAGS := -T linker.ld -ffreestanding -O2 -nostdlib -m32

SRCDIR  := src
OBJDIR  := build
SRC_C   := $(shell find $(SRCDIR) -name '*.c')
SRC_S   := $(shell find $(SRCDIR) -name '*.s')
OBJS    := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC_C)) \
	$(patsubst $(SRCDIR)/%.s,$(OBJDIR)/%.o,$(SRC_S))
DEPS    := $(OBJS:.o=.d)
KERNEL  := kernel.bin
ISO     := Vahix.iso

.PHONY: all clean iso

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

-include $(DEPS)
