/* SPDX-License-Identifier: GPL-2.0-only */
/* Copyright (C) 2026 Vahid Khalafov */

#ifndef VAHIX_SHELL_H
#define VAHIX_SHELL_H

#include <stdbool.h>
#include <stdint.h>

#include "drivers/keyboard.h"
#include "drivers/vga.h"
#include "kernel/io.h"
#include "lib/random.h"
#include "lib/string.h"
#include "drivers/sound.h"
#include "shell/vi.h"

void shell(void);

#endif /* VAHIX_SHELL_H */
