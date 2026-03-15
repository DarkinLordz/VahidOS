#pragma once

#include <stdint.h>
#include <stddef.h>

void new_line(void);
void move_cursor(int direction);
void clear(void);
void print_character(const char character);
void print_string(const char *string);

#define VGA_WIDTH 80
#define VGA_HEIGHT 25