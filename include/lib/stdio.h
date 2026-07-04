#include <stdint.h>
#include <stdarg.h>
#include <drivers/vga.h>


void print_string(const char *string);
void print_hex_byte(uint8_t value);
void print_hex(uint32_t value);
int printf(const char * format, ...);
