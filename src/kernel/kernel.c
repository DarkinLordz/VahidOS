#include "shell/shell.h"

void kernel_main(void) {
    shell();
    while (1) {
        __asm__ volatile ("hlt");
    }
}
