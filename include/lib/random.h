/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#ifndef VAHIX_RANDOM_H
#define VAHIX_RANDOM_H

#include <stdint.h>

uint32_t random(void);
uint32_t get_hwrng_seed(void);
uint32_t get_seed_from_rdtsc(void);

#endif /* VAHIX_RANDOM_H */
