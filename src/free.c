#include "malloc.h"
#include "libft.h"
#include <sys/mman.h>

static void update_first_node(t_memory_chunk *prev, t_memory_chunk *next, t_memory_chunk *chunk)
{
	if (g_memory->dynamic_chunk == chunk)
		g_memory->dynamic_chunk = next;
	if (prev != NULL)
		prev->next = next;
	if (next != NULL)
		next->prev = prev;
}

/*
 * Free a dynamic chunk.
 */
static void unmap_dynamic_chunk(t_memory_chunk *chunk)
{
	t_memory_chunk *next;
	t_memory_chunk *prev;

	next = chunk->next;
	prev = chunk->prev;

	g_memory->total_allocated_size -= chunk->allocated_size;
	munmap(chunk, chunk->projection_size);
	update_first_node(prev, next, chunk);
}

/*
 * Free a static chunk.
 */
static void unmap_static_chunk(t_memory_chunk *chunk)
{
	g_memory->total_allocated_size -= chunk->allocated_size;
	chunk->free = true;
	chunk->allocated_size = 0;
}

/*
 * My own implementation of free using mmap/munmap.
 */
void free(void *ptr)
{
	t_memory_chunk *chunk;

	pthread_mutex_lock(&g_mutex);
	if (is_allocated(ptr) == false)
		return;
	chunk = (t_memory_chunk * )((char *) ptr - sizeof(t_memory_chunk));

	if (chunk->allocation_type == STATIC_ALLOCATION)
		unmap_static_chunk(chunk);
	else
		unmap_dynamic_chunk(chunk);
	pthread_mutex_unlock(&g_mutex);
}
