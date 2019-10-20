#include "malloc.h"
#include "libft.h"

void print_total_size(void)
{
	ft_putstr("Total : ");
	ft_putnbr(memory->total_allocated_size);
	ft_putstr(" bytes\n");
}

void print_mem(char *chunk_name, t_memory_chunk *chunk)
{
	size_t	count;

	count = 0;
	ft_putstr(chunk_name);
	ft_putstr(" : ");
	print_address_hex(chunk);
	ft_putchar('\n');
	while (chunk != NULL) {
		if (chunk->free == false) {
			print_address_hex(chunk->free_space);
			ft_putstr(" - ");
			print_address_hex(chunk->free_space + chunk->allocated_size);
			ft_putstr(" : ");
			ft_putnbr(chunk->allocated_size);
			ft_putstr(" bytes\n");
			count++;
		}
		chunk = chunk->next;
	}
	ft_printf("Number of allocations : %zd\n", count);
}

void show_alloc_mem(void) {
	init_memory();
	print_mem("TINY", memory->tiny_chunk);
	print_mem("SMALL", memory->medium_chunk);
	print_mem("DYNAMIC", memory->dynamic_chunk);
	print_total_size();
}
