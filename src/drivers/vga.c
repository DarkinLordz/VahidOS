#include "drivers/vga.h"

static volatile uint16_t *const VGA_BUFFER = (uint16_t *)0xB8000;
static uint8_t VGA_COLOR = 0x0F;
size_t cursor = 0;

void update_hardware_cursor(void) {
    uint16_t pos = (uint16_t)cursor;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void move_cursor(int direction) {
    cursor += direction;
    update_hardware_cursor();
}

void set_cursor(int location) {
    cursor = location;
    update_hardware_cursor();
}

void new_line(void) {
    move_cursor(VGA_WIDTH - (cursor % VGA_WIDTH));
}

void scroll(void) {
    for (size_t i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
        VGA_BUFFER[i] = VGA_BUFFER[i + VGA_WIDTH];
    }
    for (size_t i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        VGA_BUFFER[i] = (uint16_t)' ' | ((uint16_t)VGA_COLOR << 8);
    }
    cursor = (VGA_HEIGHT - 1) * VGA_WIDTH;
    update_hardware_cursor();
}

void set_color(uint8_t fg, uint8_t bg) {
    VGA_COLOR = (fg & 0x0F) | ((bg & 0x07) << 4); // We don't touch the 7th bit
}

void blink(bool state) {
    if (state) {
        VGA_COLOR |= (1 << 7);
    } else {
        VGA_COLOR &= ~(1 << 7);
    }
}

uint8_t get_color(void) {
    return VGA_COLOR;
}

void clear(void) {
    for (size_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_BUFFER[i] = (uint16_t)' ' | ((uint16_t)VGA_COLOR << 8);
    }
    cursor = 0;
    update_hardware_cursor();
}

void print_character(const char character) {
    if (cursor >= VGA_WIDTH * VGA_HEIGHT) {
        scroll();
    }
    if (character == '\b') {
        if (cursor > 0) {
            move_cursor(-1);
            VGA_BUFFER[cursor] = (uint16_t)' ' | ((uint16_t)VGA_COLOR << 8);
        }
    } else if (character == '\n') {
        new_line();
    } else {
        VGA_BUFFER[cursor] = (uint16_t)character | ((uint16_t)VGA_COLOR << 8);
        move_cursor(1);
    }
}

void print_string(const char *string) {
    for (size_t i = 0; string[i] != '\0'; ++i) {
        print_character(string[i]);
    }
}

void print_hex_byte(uint8_t value) {
    char *hex_chars = "0123456789ABCDEF";
    print_character(hex_chars[(value >> 4) & 0x0F]);
    print_character(hex_chars[value & 0x0F]);
}

void print_hex(uint32_t value) {
    print_string("0x");
    for (int i = 3; i >= 0; i--) {
        uint8_t byte = (value >> (i * 8)) & 0xFF;
        print_hex_byte(byte);
    }
}

void change_cursor(const char cursor)
{
    char cursor_start, cursor_end;
    cursor_start = cursor & 0x0F;
    cursor_end = cursor >> 4;

    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);

}