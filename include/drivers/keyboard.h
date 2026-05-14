/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#ifndef VAHIX_KEYBOARD_H
#define VAHIX_KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

#include "kernel/io.h"

#define ARROW_KEY_UP		((uint8_t)0x80)
#define ARROW_KEY_DOWN		((uint8_t)0x81)
#define ARROW_KEY_LEFT		((uint8_t)0x82)
#define ARROW_KEY_RIGHT		((uint8_t)0x83)

void keyboard_init(void);
int keyboard_poll_char(char *out);
char keyboard_raw_read(void);
char keyboard_getchar(void);

#endif /* VAHIX_KEYBOARD_H */
