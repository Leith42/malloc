#include "malloc.h"
#include <sys/mman.h>

static t_memory_chunk *get_last_chunk(t_memory_chunk *chunk) {
	while (chunk->next != NULL)
		chunk = chunk->next;
	return chunk;
}

void *allocate_dynamic_chunk(t_memory_chunk **chunk, size_t size)
{
	t_memory_chunk *chunk_crawler;
	size_t rounded_size;

	rounded_size = align_to_page_size(size + sizeof(t_memory_chunk));
	if (*chunk == NULL)
	{
		*chunk = mmap(NULL, rounded_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (*chunk == MAP_FAILED)
			return NULL; //todo: error
		(*chunk)->prev = NULL;
		(*chunk)->next = NULL;
		(*chunk)->free_space = (char *) *chunk + sizeof(t_memory_chunk);
		(*chunk)->allocated_size = size;
		(*chunk)->projection_size = rounded_size;
		(*chunk)->allocation_type = DYNAMIC_ALLOCATION;
		(*chunk)->free = false;
		memory->total_allocated_size += size;
		return (*chunk)->free_space;
	}
	else
	{
		chunk_crawler = get_last_chunk(*chunk);
		chunk_crawler->next = mmap(NULL, rounded_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (chunk_crawler->next == MAP_FAILED)
			return NULL; //todo: error
		chunk_crawler->next->prev = chunk_crawler;
		chunk_crawler->next->next = NULL;
		chunk_crawler->next->free_space = (char *)chunk_crawler->next + sizeof(t_memory_chunk);
		chunk_crawler->next->allocated_size = size;
		chunk_crawler->next->projection_size = rounded_size;
		chunk_crawler->next->allocation_type = DYNAMIC_ALLOCATION;
		chunk_crawler->next->free = false;
		memory->total_allocated_size += size;
		return chunk_crawler->next->free_space;
	}
}

void *allocate_static_chunk(t_memory_chunk *chunk, size_t chunk_size, size_t allocated_size) {
	size_t count;

	count = 0;
	while (count < CHUNK_MAX)
	{
		if (chunk->free == true)
		{
			chunk->free = false;
			chunk->allocated_size = allocated_size;
			memory->total_allocated_size += allocated_size;
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
			memory->total_allocated_size += allocated_size;
			return chunk->next->free_space;
		}
		chunk = chunk->next;
		count++;
	}

	return NULL;
}

void *allocate_memory(size_t allocated_size) {
	void *free_chunk;

	free_chunk = NULL;

	if (allocated_size <= memory->tiny_chunk_size)
		free_chunk = allocate_static_chunk(memory->tiny_chunk, memory->tiny_chunk_size, allocated_size);
	else if (allocated_size <= memory->medium_chunk_size)
		free_chunk = allocate_static_chunk(memory->medium_chunk, memory->medium_chunk_size, allocated_size);
	if (free_chunk == NULL)
		free_chunk = allocate_dynamic_chunk(&memory->dynamic_chunk, allocated_size);

	return free_chunk;
}
