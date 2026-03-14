#pragma once

#include <stdint.h>
#include <stdbool.h>

void keyboard_init(void);
int keyboard_poll_char(char *out);
char keyboard_getchar(void);
