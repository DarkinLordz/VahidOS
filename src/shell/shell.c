/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#include "shell/shell.h"

static void cmd_help(char *args)
{
	(void)args;

	print_string("help\nclear\nhalt\ncpuid\ncolor\npeek\n"
		"dump\ncursor\nrandom\npoke\nblink");
}

static void cmd_clear(char *args)
{
	(void)args;

	clear();
}

static void cmd_halt(char *args)
{
	(void)args;

	while (1) {
		asm volatile("cli; hlt");
	}
}

static void cmd_color(char *args)
{
	char *fg_str;
	char *bg_str = NULL;
	uint8_t fg;
	uint8_t bg;
	int i;

	if (!args || !*args) {
		return;
	}

	fg_str = args;

	for (i = 0; args[i] != '\0'; i++) {
		if (args[i] == ' ') {
			args[i] = '\0';
			bg_str = &args[i + 1];
			break;
		}
	}

	if (bg_str == NULL || *bg_str == '\0') {
		return;
	}

	fg = (uint8_t)string_to_hex(fg_str);
	bg = (uint8_t)string_to_hex(bg_str);

	set_color(fg, bg);
}

static void cmd_peek(char *args)
{
	uint32_t addr;
	uint8_t *ptr;
	uint8_t value;

	if (!args || !*args) {
		return;
	}

	addr = string_to_hex(args);
	ptr = (uint8_t *)addr;
	value = *ptr;
	print_hex(value);
}

static void cmd_poke(char *args)
{
	char *addr_str;
	char *val_str = NULL;
	uint32_t address;
	uint8_t value;
	uint8_t *ptr;
	int i;

	if (!args || !*args) {
		return;
	}

	addr_str = args;

	for (i = 0; args[i] != '\0'; i++) {
		if (args[i] == ' ') {
			args[i] = '\0';
			val_str = &args[i + 1];
			break;
		}
	}

	if (val_str == NULL || *val_str == '\0') {
		return;
	}

	address = string_to_hex(addr_str);
	value = (uint8_t)string_to_hex(val_str);

	ptr = (uint8_t *)address;
	*ptr = value;
}

static void cmd_dump(char *args)
{
	uint32_t addr;
	uint8_t *ptr;
	int i;

	if (!args || !*args) {
		return;
	}

	addr = string_to_hex(args);
	ptr = (uint8_t *)addr;

	for (i = 0; i < 16; i++) {
		print_hex_byte(ptr[i]);
		print_character(' ');
		if (i == 7) {
			print_string("| ");
		}
	}
}

static void cmd_blink(char *args)
{
	bool state;

	if (!args || !*args) {
		return;
	}

	state = string_to_hex(args);

	blink(state);
}

static void cmd_cursor(char *args)
{
	uint8_t cursor_value;

	if (!args || !*args) {
		return;
	}

	cursor_value = string_to_hex(args);
	change_cursor(cursor_value);
}

static void cmd_random(char *args)
{
	uint32_t random_number;
	char random_number_string[64];

	(void)args;

	random_number = random();
	itoa(random_number, random_number_string);
	print_string(random_number_string);
}

struct shell_command {
	const char *name;
	void (*handler)(char *args);
};

static const struct shell_command commands[] = {
	{ "help", cmd_help },
	{ "clear", cmd_clear },
	{ "halt", cmd_halt },
	{ "color", cmd_color },
	{ "peek", cmd_peek },
	{ "dump", cmd_dump },
	{ "cursor", cmd_cursor },
	{ "random", cmd_random },
	{ "poke", cmd_poke },
	{ "blink", cmd_blink },
};

#define COMMAND_COUNT (sizeof(commands) / sizeof(commands[0]))

static void execute_command(char *input)
{
	char *args;
	size_t i;

	if (*input == '\0') {
		return;
	}

	args = input;

	while (*args != ' ' && *args != '\0') {
		args++;
	}

	if (*args == ' ') {
		*args = '\0';
		args++;
	}

	for (i = 0; i < COMMAND_COUNT; i++) {
		if (strcmp(input, commands[i].name) == 0) {
			commands[i].handler(args);
			return;
		}
	}

	print_string("Unknown command: ");
	print_string(input);
}

void shell(void)
{
	char input_buffer[64];
	int index = 0;
	char c;

	keyboard_init();
	clear();
	print_string("Welcome to Vahix Shell!\n");
	print_string("To see available commands, type 'help'\n");
	print_string("# ");

	while (1) {
		if (keyboard_poll_char(&c)) {
			if (c == '\n') {
				print_character('\n');
				input_buffer[index] = '\0';

				execute_command(input_buffer);

				index = 0;
				print_string("\n# ");
				continue;
			} else if (c == '\b') {
				if (index > 0) {
					index--;
					input_buffer[index] = '\0';
					print_character('\b');
				}
			} else {
				if (index < 63) {
					print_character(c);
					input_buffer[index] = c;
					index++;
					input_buffer[index] = '\0';
				}
			}
		}

		asm volatile("pause");
	}
}
