/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#include <stdbool.h>
#include <stdint.h>

#include "drivers/vga.h"
#include "shell/vi.h"
#include "drivers/keyboard.h"

typedef enum {
    Normal,
    Insert,
} Mode;

typedef struct {
    uint16_t x;
    uint16_t y;
} Cursor;

typedef struct {
    Cursor cursor;
    Mode mode;
} Editor;

void vi_run(void)
{
    clear();

    Editor editor;

    editor.cursor.x = 0;
    editor.cursor.y = 0;
    editor.mode = Normal;

    uint8_t key;

    while(true) {
        set_cursor(editor.cursor.y * VGA_WIDTH + editor.cursor.x);
        if(keyboard_poll_char(&key)) {
            if(key == 'q') {
                break;
            }
            else if(key == ' ') {
                editor.mode = Normal;
            } else if(key == ARROW_KEY_LEFT) {
                if(editor.cursor.x > 0) {
                    editor.cursor.x--;
                }
            } else if(key == ARROW_KEY_DOWN) {
                editor.cursor.y++;
            } else if(key == ARROW_KEY_UP) {
                if(editor.cursor.y > 0) {
                    editor.cursor.y--;
                }
            } else if(key == ARROW_KEY_RIGHT) {
                editor.cursor.x++;
            }
            else if(editor.mode == Normal) {
                if(key == 'i')
                {
                    editor.mode = Insert;
                } else if(key == 'h') {
                    if(editor.cursor.x > 0) {
                        editor.cursor.x--;
                    }
                }
                else if(key == 'j') {
                    editor.cursor.y++;
                } else if(key == 'k') {
                    if(editor.cursor.y > 0) {
                        editor.cursor.y--;
                    }
                }
                else if(key == 'l') {
                    editor.cursor.x++;
                }
            }
        }

        asm volatile("pause");
    }
}
