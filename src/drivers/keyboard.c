#include "drivers/keyboard.h"

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_STATUS_OUT_READY 0x01

static bool shift_active = false;
static bool is_extended = false;

static const char BASE_MAP[128] = {
    [0x02] = '1', [0x03] = '2', [0x04] = '3', [0x05] = '4', [0x06] = '5',
    [0x07] = '6', [0x08] = '7', [0x09] = '8', [0x0A] = '9', [0x0B] = '0',
    [0x0C] = '-', [0x0D] = '=', [0x0E] = '\b', [0x0F] = '\t',
    [0x10] = 'q', [0x11] = 'w', [0x12] = 'e', [0x13] = 'r', [0x14] = 't',
    [0x15] = 'y', [0x16] = 'u', [0x17] = 'i', [0x18] = 'o', [0x19] = 'p',
    [0x1A] = '[', [0x1B] = ']', [0x1C] = '\n',
    [0x1E] = 'a', [0x1F] = 's', [0x20] = 'd', [0x21] = 'f', [0x22] = 'g',
    [0x23] = 'h', [0x24] = 'j', [0x25] = 'k', [0x26] = 'l', [0x27] = ';',
    [0x28] = '\'', [0x29] = '`', [0x2B] = '\\',
    [0x2C] = 'z', [0x2D] = 'x', [0x2E] = 'c', [0x2F] = 'v', [0x30] = 'b',
    [0x31] = 'n', [0x32] = 'm', [0x33] = ',', [0x34] = '.', [0x35] = '/',
    [0x39] = ' ',
};

static const char SHIFT_MAP[128] = {
    [0x02] = '!', [0x03] = '@', [0x04] = '#', [0x05] = '$', [0x06] = '%',
    [0x07] = '^', [0x08] = '&', [0x09] = '*', [0x0A] = '(', [0x0B] = ')',
    [0x0C] = '_', [0x0D] = '+', [0x0E] = '\b', [0x0F] = '\t',
    [0x10] = 'Q', [0x11] = 'W', [0x12] = 'E', [0x13] = 'R', [0x14] = 'T',
    [0x15] = 'Y', [0x16] = 'U', [0x17] = 'I', [0x18] = 'O', [0x19] = 'P',
    [0x1A] = '{', [0x1B] = '}', [0x1C] = '\n',
    [0x1E] = 'A', [0x1F] = 'S', [0x20] = 'D', [0x21] = 'F', [0x22] = 'G',
    [0x23] = 'H', [0x24] = 'J', [0x25] = 'K', [0x26] = 'L', [0x27] = ':',
    [0x28] = '"', [0x29] = '~', [0x2B] = '|',
    [0x2C] = 'Z', [0x2D] = 'X', [0x2E] = 'C', [0x2F] = 'V', [0x30] = 'B',
    [0x31] = 'N', [0x32] = 'M', [0x33] = '<', [0x34] = '>', [0x35] = '?',
    [0x39] = ' ',
};

static const unsigned char EXTENDED_MAP[128] = {
    [0x48] = ARROW_KEY_UP,
    [0x50] = ARROW_KEY_DOWN,
    [0x4B] = ARROW_KEY_LEFT,
    [0x4D] = ARROW_KEY_RIGHT,
    [0x1C] = '\n',
    [0x35] = '/',
};

static char translate_scancode(uint8_t scancode) {
    if (is_extended)
    {
        is_extended = false;
        return (scancode < 128) ? EXTENDED_MAP[scancode] : 0;
    }
    if (scancode >= 128) return 0;

    const char *map = shift_active ? SHIFT_MAP : BASE_MAP;
    return map[scancode];
}

void keyboard_init(void) {
    shift_active = false;

    while (inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_OUT_READY) {
        (void)inb(KEYBOARD_DATA_PORT);
    }
}

char keyboard_raw_read(void)
{
    if (!(inb(KEYBOARD_STATUS_PORT) & KEYBOARD_STATUS_OUT_READY))
    {
        return 0;
    }

    return inb(KEYBOARD_DATA_PORT);
}

int keyboard_poll_char(char *out) {

    uint8_t scancode = keyboard_raw_read();

    if (scancode == 0xE0) {
        is_extended = true;
        return 0;
    }
    
    if (!is_extended)
    {
        if (scancode == 0x2A || scancode == 0x36) {
            shift_active = true;
            return 0;
        }
        if (scancode == 0xAA || scancode == 0xB6) {
            shift_active = false;
            return 0;
        }
    }

    if (scancode & 0x80) {
        is_extended = false;
        return 0;
    }

    char translated = translate_scancode(scancode);
    if (translated == 0) {
        return 0;
    }

    *out = translated;
    return 1;
}
