/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

ENTRY(_start)

SECTIONS {
	. = 1M;

	.multiboot : {
		*(.multiboot)
	}

	.text : {
		*(.text*)
	}

	.rodata : {
		*(.rodata*)
	}

	.data : {
		*(.data*)
	}

	.bss : {
		*(COMMON)
		*(.bss*)
	}
}
