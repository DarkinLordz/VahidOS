#include <kernel/shell.h>
#include <drivers/keyboard.h>
#include <drivers/vga.h>

void kernel_main(void) {
    keyboard_init();
    clear();
    shell();
    while (1) {
        __asm__ volatile ("hlt");
    }
}
