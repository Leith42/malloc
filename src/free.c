#include "malloc.h"
#include <sys/mman.h>

static void update_first_node(t_memory_chunk *prev, t_memory_chunk *next, t_memory_chunk *chunk)
{
	if (memory->dynamic_chunk == chunk)
	{
		memory->dynamic_chunk = next;
	}
	if (prev != NULL)
	{
		prev->next = next;
	}
	if (next != NULL)
	{
		next->prev = prev;
	}
}

static void unmap_dynamic_chunk(t_memory_chunk *chunk) {
	t_memory_chunk *next;
	t_memory_chunk *prev;

	next = chunk->next;
	prev = chunk->prev;

	memory->total_allocated_size -= chunk->allocated_size;
	memory->allocation_count--;
	munmap(chunk, chunk->projection_size);
	update_first_node(prev, next, chunk);
}

static void unmap_static_chunk(t_memory_chunk *chunk) {
	memory->total_allocated_size -= chunk->allocated_size;
	chunk->free = true;
	chunk->allocated_size = 0;
	memory->allocation_count--;
}

void free(void *ptr) {
	t_memory_chunk *chunk;

	if (ptr == NULL)
		return;
	chunk = (t_memory_chunk * )((char *) ptr - sizeof(t_memory_chunk));

	if (chunk->allocation_type == STATIC_ALLOCATION)
		unmap_static_chunk(chunk);
	else
		unmap_dynamic_chunk(chunk);
	if (memory->allocation_count == 0)
	{
		munmap(memory, memory->projection_size);
		memory = NULL;
	}
}
