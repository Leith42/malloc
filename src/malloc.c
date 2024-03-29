#include "malloc.h"

/*
 * My own implementation of malloc using mmap/munmap.
 */
void *malloc(size_t size) {
	void *allocation;

	if (size == 0)
		return (NULL);

	pthread_mutex_lock(&g_mutex);
	if (init_memory() != -1)
		allocation = allocate_memory(size);
	else
		allocation = NULL;
	pthread_mutex_unlock(&g_mutex);
	
	return allocation;
}
