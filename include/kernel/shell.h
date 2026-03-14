#pragma once

#include <stdint.h>

#include <lib/string.h>
#include <drivers/keyboard.h>
#include <drivers/vga.h>

void execute_command(char *command);
void shell(void);