/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#include <stdbool.h>
#include <stdint.h>

#include "drivers/vga.h"
#include "shell/vi.h"
#include "drivers/keyboard.h"

typedef struct {
    uint16_t x;
    uint16_t y;
} Cursor;

typedef struct {
    Cursor cursor;
} Editor;

void vi_run(void)
{
    clear();

    Editor editor;

    editor.cursor.x = 0;
    editor.cursor.y = 0;

    uint8_t key;

    while(true) {
        set_cursor(editor.cursor.y * VGA_WIDTH + editor.cursor.x);
        if(keyboard_poll_char(&key)) {
            if(key == 'q') {
                break;
            }
        }

        asm volatile("pause");
    }
}
