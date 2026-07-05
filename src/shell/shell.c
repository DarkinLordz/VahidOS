/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#include "shell/shell.h"

#define SHELL_INPUT_MAX 63

static void cmd_help(char *args)
{
	(void)args;

	print_string("help\nclear\nhalt\ncolor\npeek\n"
		"dump\ncursor\nrandom\npoke\nblink\nbeep\nvi");
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
	printf("%d", random_number);
}

static void cmd_beep(char *args)
{
    uint32_t freq = 440;
    uint32_t dur  = 1000;

	(void)args;

    pc_speaker_beep(freq, dur);
}

static void cmd_vi(char *args)
{
	(void)args;

	vi_run();
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
	{ "beep", cmd_beep },
	{ "vi", cmd_vi}
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

	printf("Unknown command: %s ", input);

}

void shell(void)
{
	char input_buffer[SHELL_INPUT_MAX + 1];
	int length = 0;
	int cursor_index = 0;
	uint8_t key;
	int i;

	keyboard_init();
	clear();
	printf("Welcome to Vahix Shell!\n");
	printf("To see available commands, type 'help'\n");
	printf("# ");

	while (1) {
		if (keyboard_poll_char(&key)) {
			if (key == '\n') {
				if (cursor_index < length) {
					move_cursor(length - cursor_index);
				}

				print_character('\n');
				input_buffer[length] = '\0';

				execute_command(input_buffer);

				length = 0;
				cursor_index = 0;
				print_string("\n# ");
				continue;
			} else if (key == '\b') {
				if (cursor_index > 0) {
					cursor_index--;
					length--;


					for (i = cursor_index; i < length; i++) {
						input_buffer[i] = input_buffer[i + 1];
					}

					input_buffer[length] = '\0';

					move_cursor(-1);
					for (i = cursor_index; i < length; i++) {
						print_character(input_buffer[i]);
					}
					print_character(' ');
					move_cursor(-(length - cursor_index + 1));
				}
			} else if (key == ARROW_KEY_LEFT) {
				if (cursor_index > 0) {
					cursor_index--;
					move_cursor(-1);
				}
			} else if (key == ARROW_KEY_RIGHT) {
				if (cursor_index < length) {
					cursor_index++;
					move_cursor(1);
				}
			} else if (key == ARROW_KEY_UP || key == ARROW_KEY_DOWN) {
				continue;
			} else {
				if (length < SHELL_INPUT_MAX) {
					for (i = length; i > cursor_index; i--) {
						input_buffer[i] = input_buffer[i - 1];
					}

					input_buffer[cursor_index] = (char)key;
					length++;
					input_buffer[length] = '\0';

					for (i = cursor_index; i < length; i++) {
						print_character(input_buffer[i]);
					}

					cursor_index++;
					if (cursor_index < length) {
						move_cursor(-(length - cursor_index));
					}
				}
			}
		}

		asm volatile("pause");
	}
}
