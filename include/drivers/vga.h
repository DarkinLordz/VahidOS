/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#ifndef VAHIX_VGA_H
#define VAHIX_VGA_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kernel/io.h"

#define VGA_WIDTH	80
#define VGA_HEIGHT	25

void new_line(void);
void move_cursor(int direction);
void set_cursor(int location);
void clear(void);
void print_character(const char character);
void print_string(const char *string);
void print_hex_byte(uint8_t value);
void print_hex(uint32_t value);
void scroll(void);
void set_color(uint8_t fg, uint8_t bg);
uint8_t get_color(void);
void change_cursor(const char cursor_shape);
void blink(bool state);
void write_cell(size_t row, size_t col, char character, uint8_t color);

#endif /* VAHIX_VGA_H */
