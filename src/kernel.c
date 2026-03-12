#include <stddef.h>
#include <stdint.h>

static volatile uint16_t *const VGA_BUFFER = (uint16_t *)0xB8000;
static const uint8_t VGA_COLOR = 0x0F;
size_t cursor = 0;

void print(const char *msg) {
    for (size_t i = 0; msg[i] != '\0'; ++i) {
        if (msg[i] == '\n'){
            new_line();
        } else { // This will lead to triple fault when cursor exceeds 2000
            VGA_BUFFER[cursor++] = (uint16_t)msg[i] | ((uint16_t)VGA_COLOR << 8);
        }
    }
}

void clear(void) {
    for (size_t i = 0; i < 80 * 25; i++) {
        VGA_BUFFER[i] = (uint16_t)' ' | ((uint16_t)VGA_COLOR << 8);
    }
    cursor = 0;
}

void new_line(void) {
    cursor += 80 - (cursor % 80);
}

void kernel_main(void) {
    for (;;) {
        __asm__ volatile ("hlt");
    }
}
