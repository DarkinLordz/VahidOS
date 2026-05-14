/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#include "lib/string.h"

int strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}

	return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

size_t strlen(const char *s)
{
	const char *p = s;

	while (*p) {
		p++;
	}

	return (size_t)(p - s);
}

int strncmp(const char *s1, const char *s2, size_t n)
{
	while (n && *s1 && (*s1 == *s2)) {
		s1++;
		s2++;
		n--;
	}

	if (n == 0) {
		return 0;
	}

	return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

uint32_t string_to_hex(char *str)
{
	uint32_t val = 0;
	uint8_t byte;

	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
		str += 2;
	}

	while (*str) {
		byte = *str;
		if (byte >= '0' && byte <= '9') {
			byte = byte - '0';
		} else if (byte >= 'a' && byte <= 'f') {
			byte = byte - 'a' + 10;
		} else if (byte >= 'A' && byte <= 'F') {
			byte = byte - 'A' + 10;
		}

		val = (val << 4) | (byte & 0xf);
		str++;
	}

	return val;
}

void itoa(int n, char *str)
{
	int i = 0;
	int rem;

	if (n == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return;
	}

	while (n > 0) {
		rem = n % 10;
		str[i++] = rem + '0';
		n = n / 10;
	}

	str[i] = '\0';

	reverse(str, i);
}

void reverse(char *str, int length)
{
	int start = 0;
	int end = length - 1;
	char temp;

	while (start < end) {
		temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		start++;
		end--;
	}
}

int atoi(const char *nptr)
{
	int value = 0;
	int i;

	for (i = 0; nptr[i] != '\0'; i++) {
		if (nptr[i] >= '0' && nptr[i] <= '9') {
			value = (value * 10) + (nptr[i] - '0');
		} else {
			break;
		}
	}

	return value;
}
