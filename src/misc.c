#include "malloc.h"
#include "libft.h"
#include <stdio.h>
#include <stdint.h>

/*
 * Return digit in hexadecimal.
 */
char hex_digit(int v)
{
	if (v >= 0 && v < 10)
		return '0' + v;
	else
		return 'A' + v - 10;
}

/*
 * Print address in hexadecimal.
 */
void print_address_hex(void* address)
{
	int i;
	uintptr_t p;
	char hex_char;
	bool leading_zero;

	p = (uintptr_t)address;
	leading_zero = true;
	if (address == NULL)
	{
		ft_putstr("0x0");
	}
	else
	{
		i = (sizeof(p) << 3) - 4;
		ft_putstr("0x");
		while (i >= 0)
		{
			hex_char = hex_digit((p >> i) & 0xf);
			if (hex_char != '0' && leading_zero == true)
				leading_zero = false;
			if (leading_zero == false)
				ft_putchar(hex_char);
			i -= 4;
		}
	}
}

/*
 * Rounds size to the nearest larger multiple of system page size.
 */
size_t	align_to_page_size(size_t size)
{
	size_t system_page_size;

	system_page_size = getpagesize();
	return (((size / system_page_size) * system_page_size) + system_page_size);
}
