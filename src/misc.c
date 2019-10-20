#include "malloc.h"
#include "libft.h"
#include <stdio.h>
#include <stdint.h>

char hex_digit(int v) {
	if (v >= 0 && v < 10)
		return '0' + v;
	else
		return 'A' + v - 10;
}

void print_address_hex(void* p0) {
	int i;
	uintptr_t p = (uintptr_t)p0;
	char hex_char;
	bool leading_zero = true;

	if (p0 == NULL)
	{
		ft_putstr("0x0");
	}
	else
	{
		ft_putstr("0x");
		for(i = (sizeof(p) << 3) - 4; i>=0; i -= 4) {
			hex_char = hex_digit((p >> i) & 0xf);
			if (hex_char != '0' && leading_zero == true)
				leading_zero = false;
			if (leading_zero == false)
				ft_putchar(hex_char);
		}
	}
}

/*
 * Rounds size to the nearest larger multiple of system page size.
 */
size_t	align_to_page_size(size_t size)
{
	return (((size / memory->system_page_size) * memory->system_page_size) + memory->system_page_size);
}
