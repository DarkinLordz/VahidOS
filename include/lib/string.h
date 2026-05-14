/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#ifndef VAHIX_STRING_H
#define VAHIX_STRING_H

#include <stddef.h>
#include <stdint.h>

#include "drivers/vga.h"

int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);
size_t strlen(const char *s);
uint32_t string_to_hex(char *str);
void itoa(int n, char *str);
void reverse(char *str, int length);
int atoi(const char *nptr);

#endif /* VAHIX_STRING_H */
