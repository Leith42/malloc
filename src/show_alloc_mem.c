#include "malloc.h"
#include "libft.h"

static void print_total_size(void)
{
	ft_putstr("Total : ");
	ft_putnbr(g_memory->total_allocated_size);
	ft_putstr(" bytes\n");
}

static void print_mem(char *chunk_name, t_memory_chunk *chunk)
{
	size_t	count;

	count = 0;
	ft_putstr(chunk_name);
	ft_putstr(" : ");
	print_address_hex(chunk);
	ft_putchar('\n');
	while (chunk != NULL) {
		if (chunk->free == false)
		{
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
}

/*
 * Prints state of the allocated memory chunks.
 */
void show_alloc_mem(void) {
	pthread_mutex_lock(&g_mutex);
	if (init_memory() != -1)
	{
		print_mem("TINY", g_memory->tiny_chunk);
		print_mem("SMALL", g_memory->medium_chunk);
		print_mem("LARGE", g_memory->dynamic_chunk);
		print_total_size();
	}
	pthread_mutex_unlock(&g_mutex);
}
