#include <lib/stdio.h>

void print_string(const char *string)
{
	size_t i;

	for (i = 0; string[i] != '\0'; i++) {
		print_character(string[i]);
	}
}

void print_hex_byte(uint8_t value)
{
	const char *hex_chars = "0123456789ABCDEF";

	print_character(hex_chars[(value >> 4) & 0x0f]);
	print_character(hex_chars[value & 0x0f]);
}

void print_hex(uint32_t value)
{
	uint8_t byte;
	int i;

	print_string("0x");

	for (i = 3; i >= 0; i--) {
		byte = (value >> (i * 8)) & 0xff;
		print_hex_byte(byte);
	}
}