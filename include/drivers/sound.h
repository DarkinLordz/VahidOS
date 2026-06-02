/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#ifndef VAHIX_SOUND_H
#define VAHIX_SOUND_H

#include <stdint.h>

#include "kernel/io.h"

void delay_ms(uint32_t ms);
void pc_speaker_beep(uint32_t freq_hz, uint32_t duration_ms);

#endif /* VAHIX_SOUND_H */