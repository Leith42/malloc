#include "malloc.h"
#include <sys/mman.h>

/*
 * Return the last node of chunk.
 */
static t_memory_chunk *get_last_chunk(t_memory_chunk *chunk)
{
	while (chunk->next != NULL)
		chunk = chunk->next;
	return chunk;
}

/*
 * Return an allocated memory space using mmap.
 */
void *allocate_dynamic_chunk(t_memory_chunk **chunk, size_t size)
{
	t_memory_chunk *chunk_crawler;
	size_t rounded_size;

	rounded_size = align_to_page_size(size + sizeof(t_memory_chunk));
	if (*chunk == NULL)
	{
		*chunk = mmap(NULL, rounded_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (*chunk == MAP_FAILED)
			return NULL;
		(*chunk)->prev = NULL;
		(*chunk)->next = NULL;
		(*chunk)->free_space = (char *) *chunk + sizeof(t_memory_chunk);
		(*chunk)->allocated_size = size;
		(*chunk)->projection_size = rounded_size;
		(*chunk)->allocation_type = DYNAMIC_ALLOCATION;
		(*chunk)->free = false;
		g_memory->total_allocated_size += size;
		return (*chunk)->free_space;
	}
	else
	{
		chunk_crawler = get_last_chunk(*chunk);
		chunk_crawler->next = mmap(NULL, rounded_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (chunk_crawler->next == MAP_FAILED)
			return NULL;
		chunk_crawler->next->prev = chunk_crawler;
		chunk_crawler->next->next = NULL;
		chunk_crawler->next->free_space = (char *)chunk_crawler->next + sizeof(t_memory_chunk);
		chunk_crawler->next->allocated_size = size;
		chunk_crawler->next->projection_size = rounded_size;
		chunk_crawler->next->allocation_type = DYNAMIC_ALLOCATION;
		chunk_crawler->next->free = false;
		g_memory->total_allocated_size += size;
		return chunk_crawler->next->free_space;
	}
}
/*
 * Return a pre-allocated memory space.
 */
void *allocate_static_chunk(t_memory_chunk *chunk, size_t chunk_size, size_t allocated_size) {
	size_t count;

	count = 0;
	while (count < CHUNK_MAX)
	{
		if (chunk->free == true)
		{
			chunk->free = false;
			chunk->allocated_size = allocated_size;
			g_memory->total_allocated_size += allocated_size;
			return chunk->free_space;
		}
		else if (chunk->next == NULL && count < (CHUNK_MAX - 1))
		{
			chunk->next = (t_memory_chunk * )(chunk->free_space + chunk_size);
			chunk->next->prev = chunk;
			chunk->next->next = NULL;
			chunk->next->allocation_type = STATIC_ALLOCATION;
			chunk->next->free = false;
			chunk->next->allocated_size = allocated_size;
			chunk->next->projection_size = sizeof(t_memory_chunk) + chunk_size;
			chunk->next->free_space = (char *)chunk->next + sizeof(t_memory_chunk);
			g_memory->total_allocated_size += allocated_size;
			return chunk->next->free_space;
		}
		chunk = chunk->next;
		count++;
	}

	return NULL;
}

/*
 * Return an allocated memory space according to allocated_size.
 */
void *allocate_memory(size_t allocated_size)
{
	void *ptr;

	ptr = NULL;

	if (allocated_size <= g_memory->tiny_chunk_size)
	{
		if (g_memory->tiny_chunk == NULL)
			init_tiny_chunk();
		ptr = allocate_static_chunk(g_memory->tiny_chunk, g_memory->tiny_chunk_size, allocated_size);
	}
	else if (allocated_size <= g_memory->medium_chunk_size)
	{
		if (g_memory->medium_chunk == NULL)
			init_medium_chunk();
		ptr = allocate_static_chunk(g_memory->medium_chunk, g_memory->medium_chunk_size, allocated_size);
	}
	if (ptr == NULL)
		ptr = allocate_dynamic_chunk(&g_memory->dynamic_chunk, allocated_size);

	return ptr;
}
