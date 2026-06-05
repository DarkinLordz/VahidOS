/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#include "shell/vi.h"

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
    char buffer[1024];
    uint16_t buffer_len;
} Editor;

void vi_run(void)
{
    clear();

    Editor editor;

    editor.cursor.x = 0;
    editor.cursor.y = 0;
    editor.mode = Normal;
    editor.buffer[0] = '\0';
    editor.buffer_len = 0;

    uint8_t key;

    while(true) {
        set_cursor(editor.cursor.y * VGA_WIDTH + editor.cursor.x);
        if(keyboard_poll_char(&key)) {
            if(key == '\x1b') {
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
                if(key == 'q') {
                    break;
                }
                else if(key == 'i')
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
            else if(editor.mode == Insert) {
                if(editor.buffer_len < 1023) {
                    editor.buffer[editor.buffer_len] = (char)key;
                    editor.buffer_len++;
                    editor.buffer[editor.buffer_len] = '\0';
                }
                print_character(key);
                if(key == '\b') {
                    if(editor.cursor.x > 0) {
                        editor.cursor.x--;
                    }
                } else if(key == '\n') {
                    editor.cursor.x = 0;
                    editor.cursor.y++;
                } else {
                    editor.cursor.x++;
                }
            }
        }

        asm volatile("pause");
    }
}
