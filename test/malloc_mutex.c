#include "malloc.h"
#include "libft.h"
#include <errno.h>
#include <sys/mman.h>

int counter = 0;

/*
 * My own implementation of malloc using mmap/munmap.
 */
void *malloc(size_t size) {
	void *allocation;

	if (size == 0)
		return (NULL);

	pthread_mutex_lock(&g_mutex);
	counter++;
	ft_putnbr(counter);
	ft_putstr(" has started\n");
	if (init_memory() != -1)
		allocation = allocate_memory(size);
	else
		allocation = NULL;
	for (size_t i = 0; i < (0xFFFFFFFF); i++);
	ft_putnbr(counter);
	ft_putstr(" has finished\n");
	pthread_mutex_unlock(&g_mutex);
	return allocation;
}
