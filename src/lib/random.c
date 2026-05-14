/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#include "lib/random.h"

uint32_t get_hwrng_seed(void)
{
	uint32_t seed;

	/*
	 * RDRAND is not available in QEMU here, but should work on
	 * hardware with RDRAND support.
	 */
	asm volatile("rdrand %0" : "=r" (seed));

	return seed;
}

uint32_t get_seed_from_rdtsc(void)
{
	uint32_t low;
	uint32_t high;

	/* RDTSC returns the cycle count split across EDX:EAX. */
	asm volatile("rdtsc" : "=a" (low), "=d" (high));

	return low;
}

uint32_t random(void)
{
	uint32_t random_number;

	random_number = get_seed_from_rdtsc();
	random_number = (random_number * 1103515245 + 12345) & 0x7fffffff;

	return random_number;
}
