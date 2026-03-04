#include <stddef.h>
#include <stdint.h>

static volatile uint16_t *const VGA_BUFFER = (uint16_t *)0xB8000;
static const uint8_t VGA_COLOR = 0x0F; /* White on black */

void kernel_main(void) {
    const char *msg = "Hello from VahidOS kernel!";

    for (size_t i = 0; msg[i] != '\0'; ++i) {
        VGA_BUFFER[i] = (uint16_t)msg[i] | ((uint16_t)VGA_COLOR << 8);
    }

    for (;;) {
        __asm__ volatile ("hlt");
    }
}
