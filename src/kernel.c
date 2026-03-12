#include <stddef.h>
#include <stdint.h>

static volatile uint16_t *const VGA_BUFFER = (uint16_t *)0xB8000;
static const uint8_t VGA_COLOR = 0x0F;

void print(const char *msg) {
    for (size_t i = 0; msg[i] != '\0'; ++i) {
        VGA_BUFFER[i] = (uint16_t)msg[i] | ((uint16_t)VGA_COLOR << 8);
    }
}

void kernel_main(void) {
    for (;;) {
        __asm__ volatile ("hlt");
    }
}
