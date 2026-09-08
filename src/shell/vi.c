/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#include "shell/vi.h"

#define VI_STATUS_ROW		(VGA_HEIGHT - 1)
#define VI_TEXT_HEIGHT		(VGA_HEIGHT - 1)
#define VI_BUFFER_SIZE		(VGA_WIDTH * VI_TEXT_HEIGHT)
#define VI_TAB_WIDTH		4

typedef enum {
	MODE_NORMAL,
	MODE_INSERT,
} Mode;

typedef struct {
	uint16_t x;
	uint16_t y;
} Cursor;

typedef struct {
	Cursor cursor;
	Mode mode;
	char buffer[VI_BUFFER_SIZE];
} Editor;

static size_t cursor_index(Cursor cursor)
{
	return cursor.y * VGA_WIDTH + cursor.x;
}

static void set_editor_cursor(const Editor *editor)
{
	set_cursor((int)cursor_index(editor->cursor));
}

static void clear_status(void)
{
	size_t i;

	for (i = 0; i < VGA_WIDTH; i++) {
		write_cell(VI_STATUS_ROW, i, ' ', get_color());
	}
}

static void display_mode(const Editor *editor)
{
	const char *label;

	label = editor->mode == MODE_NORMAL ? "-- NORMAL --" : "-- INSERT --";

	clear_status();
	set_cursor(VI_STATUS_ROW * VGA_WIDTH);
	print_string(label);
	set_editor_cursor(editor);
}

static void init_editor(Editor *editor)
{
	size_t i;

	editor->cursor.x = 0;
	editor->cursor.y = 0;
	editor->mode = MODE_NORMAL;

	for (i = 0; i < VI_BUFFER_SIZE; i++) {
		editor->buffer[i] = ' ';
	}
}

static bool move_left(Cursor *cursor)
{
	if (cursor->x == 0) {
		return false;
	}

	cursor->x--;
	return true;
}

static bool move_right(Cursor *cursor)
{
	if (cursor->x >= VGA_WIDTH - 1) {
		return false;
	}

	cursor->x++;
	return true;
}

static bool move_up(Cursor *cursor)
{
	if (cursor->y == 0) {
		return false;
	}

	cursor->y--;
	return true;
}

static bool move_down(Cursor *cursor)
{
	if (cursor->y >= VI_TEXT_HEIGHT - 1) {
		return false;
	}

	cursor->y++;
	return true;
}

static void move_next(Cursor *cursor)
{
	if (move_right(cursor)) {
		return;
	}

	if (cursor->y < VI_TEXT_HEIGHT - 1) {
		cursor->x = 0;
		cursor->y++;
	}
}

static void handle_move(Editor *editor, uint8_t key)
{
	if (key == ARROW_KEY_LEFT || key == 'h') {
		move_left(&editor->cursor);
	} else if (key == ARROW_KEY_RIGHT || key == 'l') {
		move_right(&editor->cursor);
	} else if (key == ARROW_KEY_UP || key == 'k') {
		move_up(&editor->cursor);
	} else if (key == ARROW_KEY_DOWN || key == 'j') {
		move_down(&editor->cursor);
	}
}

static void put_editor_char(Editor *editor, char character)
{
	editor->buffer[cursor_index(editor->cursor)] = character;
	write_cell(editor->cursor.y, editor->cursor.x, character, get_color());
	move_next(&editor->cursor);
}

static void insert_tab(Editor *editor)
{
	uint8_t i;

	for (i = 0; i < VI_TAB_WIDTH; i++) {
		put_editor_char(editor, ' ');
	}
}

static void insert_backspace(Editor *editor)
{
	if (!move_left(&editor->cursor)) {
		return;
	}

	editor->buffer[cursor_index(editor->cursor)] = ' ';
	write_cell(editor->cursor.y, editor->cursor.x, ' ', get_color());
}

static void insert_newline(Editor *editor)
{
	if (editor->cursor.y >= VI_TEXT_HEIGHT - 1) {
		return;
	}

	editor->cursor.x = 0;
	editor->cursor.y++;
}

static bool handle_normal(Editor *editor, uint8_t key)
{
	if (key == 'q') {
		clear();
		return false;
	}

	if (key == 'i') {
		editor->mode = MODE_INSERT;
		display_mode(editor);
	} else if (key == 'a') {
		move_right(&editor->cursor);
		editor->mode = MODE_INSERT;
		display_mode(editor);
	} else if (key == 'x') {
        editor->buffer[cursor_index(editor->cursor)] = ' ';
	    write_cell(editor->cursor.y, editor->cursor.x, ' ', get_color());
    } else {
		handle_move(editor, key);
	}

	return true;
}

static void handle_insert(Editor *editor, uint8_t key)
{
	if (key == '\t') {
		insert_tab(editor);
	} else if (key == '\b') {
		insert_backspace(editor);
	} else if (key == '\n') {
		insert_newline(editor);
	} else if (key >= ' ') { /* Printable characters only :) */
		put_editor_char(editor, (char)key);
	}
}

void vi_run(void)
{
	Editor editor;
	uint8_t key;

	clear();
	init_editor(&editor);
	display_mode(&editor);

	while (true) {
		set_editor_cursor(&editor);

		if (keyboard_poll_char(&key)) {
			if (key == '\x1b') {
				editor.mode = MODE_NORMAL;
				display_mode(&editor);
			} else if (key >= ARROW_KEY_UP && key <= ARROW_KEY_RIGHT) {
				handle_move(&editor, key);
			} else if (editor.mode == MODE_NORMAL) {
				if (!handle_normal(&editor, key)) {
					break;
				}
			} else {
				handle_insert(&editor, key);
			}
		}

		asm volatile("pause");
	}
}
