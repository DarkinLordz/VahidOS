/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#include "kernel/io.h"

void outb(uint16_t port, uint8_t val)
{
	asm volatile("outb %0, %1" : : "a" (val), "Nd" (port));
}

uint8_t inb(uint16_t port)
{
	uint8_t ret;

	asm volatile("inb %1, %0" : "=a" (ret) : "Nd" (port));

	return ret;
}
