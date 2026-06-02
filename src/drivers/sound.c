#include "drivers/sound.h"

void delay_ms(uint32_t ms)
{
    volatile uint32_t i;
    while (ms--) {
        for (i = 0; i < 200000; i++) {
            asm volatile ("pause");
        }
    }
}

void pc_speaker_beep(uint32_t freq_hz, uint32_t duration_ms)
{
    if (freq_hz == 0) return;

    uint32_t divisor = 1193180 / freq_hz;

    outb(0x43, 0xB6);
    outb(0x42, divisor & 0xFF);
    outb(0x42, (divisor >> 8) & 0xFF);
    uint8_t tmp = inb(0x61);
    outb(0x61, tmp | 0x03);

    delay_ms(duration_ms);

    tmp = inb(0x61);
    outb(0x61, tmp & ~0x03);
}