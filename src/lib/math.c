/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#include "lib/math.h"

float power(float base, int exp)
{
	float result = 1.0;
	int i;

	for (i = 0; i < exp; i++) {
		result *= base;
	}

	return result;
}
