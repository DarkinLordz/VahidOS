/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#include "shell/shell.h"

void kernel_main(void)
{
	shell();

	while (1) {
		asm volatile("hlt");
	}
}
