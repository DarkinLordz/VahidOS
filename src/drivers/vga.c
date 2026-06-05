/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#include "drivers/vga.h"

static volatile uint16_t *const vga_buffer = (uint16_t *)0xb8000;
static uint8_t vga_color = 0x0f;
static size_t cursor_pos;

static void update_hardware_cursor(void)
{
	uint16_t pos = (uint16_t)cursor_pos;

	outb(0x3d4, 0x0f);
	outb(0x3d5, (uint8_t)(pos & 0xff));
	outb(0x3d4, 0x0e);
	outb(0x3d5, (uint8_t)((pos >> 8) & 0xff));
}

void move_cursor(int direction)
{
	cursor_pos += direction;
	update_hardware_cursor();
}

void set_cursor(int location)
{
	cursor_pos = location;
	update_hardware_cursor();
}

void new_line(void)
{
	move_cursor(VGA_WIDTH - (cursor_pos % VGA_WIDTH));
}

void scroll(void)
{
	size_t i;

	for (i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
		vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
	}

	for (i = (VGA_HEIGHT - 1) * VGA_WIDTH;
	     i < VGA_HEIGHT * VGA_WIDTH;
	     i++) {
		vga_buffer[i] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
	}

	cursor_pos = (VGA_HEIGHT - 1) * VGA_WIDTH;
	update_hardware_cursor();
}

void set_color(uint8_t fg, uint8_t bg)
{
	/* Keep bit 7 available for blink control. */
	vga_color = (fg & 0x0f) | ((bg & 0x07) << 4);
}

void write_cell(size_t row, size_t col, char character, uint8_t color)
{
	if (row < VGA_HEIGHT && col < VGA_WIDTH) {
		size_t index = row * VGA_WIDTH + col;
        vga_buffer[index] = (uint16_t)character | ((uint16_t)color << 8);
	}
}

void blink(bool state)
{
	if (state) {
		vga_color |= (1 << 7);
	} else {
		vga_color &= ~(1 << 7);
	}
}

uint8_t get_color(void)
{
	return vga_color;
}

void clear(void)
{
	size_t i;

	for (i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
		vga_buffer[i] = (uint16_t)' ' | ((uint16_t)vga_color << 8);
	}

	cursor_pos = 0;
	update_hardware_cursor();
}

void print_character(const char character)
{
	if (cursor_pos >= VGA_WIDTH * VGA_HEIGHT) {
		scroll();
	}

	if (character == '\b') {
		if (cursor_pos > 0) {
			move_cursor(-1);
			vga_buffer[cursor_pos] =
				(uint16_t)' ' | ((uint16_t)vga_color << 8);
		}
	} else if (character == '\n') {
		new_line();
	} else {
		vga_buffer[cursor_pos] =
			(uint16_t)character | ((uint16_t)vga_color << 8);
		move_cursor(1);
	}
}

void print_string(const char *string)
{
	size_t i;

	for (i = 0; string[i] != '\0'; i++) {
		print_character(string[i]);
	}
}

void print_hex_byte(uint8_t value)
{
	const char *hex_chars = "0123456789ABCDEF";

	print_character(hex_chars[(value >> 4) & 0x0f]);
	print_character(hex_chars[value & 0x0f]);
}

void print_hex(uint32_t value)
{
	uint8_t byte;
	int i;

	print_string("0x");

	for (i = 3; i >= 0; i--) {
		byte = (value >> (i * 8)) & 0xff;
		print_hex_byte(byte);
	}
}

void change_cursor(const char cursor_shape)
{
	char cursor_start;
	char cursor_end;

	cursor_start = cursor_shape & 0x0f;
	cursor_end = cursor_shape >> 4;

	outb(0x3d4, 0x0a);
	outb(0x3d5, (inb(0x3d5) & 0xc0) | cursor_start);

	outb(0x3d4, 0x0b);
	outb(0x3d5, (inb(0x3d5) & 0xe0) | cursor_end);
}
