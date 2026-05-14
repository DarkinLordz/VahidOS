/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

.set ALIGN,	1 << 0
.set MEMINFO,	1 << 1
.set FLAGS,	ALIGN | MEMINFO
.set MAGIC,	0x1badb002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot, "a"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text
.global _start
.extern kernel_main

_start:
	cli
	mov $stack_top, %esp
	call kernel_main

hang:
	hlt
	jmp hang

.section .note.GNU-stack, "", @progbits
