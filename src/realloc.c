#include "malloc.h"
#include "libft.h"

/*
 * Free and copy memory from one memory space to another.
 */
static void 	chunk_cpy(t_memory_chunk *chunk, void *dest, size_t size)
{
	if (chunk != NULL)
	{
		ft_memcpy(dest, chunk->free_space, chunk->allocated_size > size ? size : chunk->allocated_size);
		free(chunk->free_space);
	}
}

/*
 * Return a reallocated memory space.
 */
static void		*realloc_dynamic_allocation(t_memory_chunk *chunk, size_t size)
{
	void *ptr;

	if (size <= g_memory->tiny_chunk_size)
	{
		if ((ptr = allocate_static_chunk(g_memory->tiny_chunk, g_memory->tiny_chunk_size, size)) == NULL)
			ptr = allocate_dynamic_chunk(&g_memory->dynamic_chunk, size);
	}
	else if (size <= g_memory->medium_chunk_size)
	{
		if ((ptr = allocate_static_chunk(g_memory->medium_chunk, g_memory->medium_chunk_size, size)) == NULL)
			ptr = allocate_dynamic_chunk(&g_memory->dynamic_chunk, size);
	}
	else
	{
		ptr = allocate_dynamic_chunk(&g_memory->dynamic_chunk, size);
	}
	chunk_cpy(chunk, ptr, size);

	return (ptr);
}

/*
 * Return a reallocated memory space.
 */
static void		*realloc_static_allocation(t_memory_chunk *chunk, size_t size)
{
	void *ptr;

	if (chunk < g_memory->medium_chunk)
	{
		if (size <= g_memory->tiny_chunk_size)
		{
			g_memory->total_allocated_size += (size - chunk->allocated_size);
			chunk->allocated_size = size;
			return chunk->free_space;
		}
		else if (size <= g_memory->medium_chunk_size)
		{
			if ((ptr = allocate_static_chunk(g_memory->medium_chunk, g_memory->medium_chunk_size, size)) == NULL)
				ptr = allocate_dynamic_chunk(&g_memory->dynamic_chunk, size);
		}
		else
			ptr = allocate_dynamic_chunk(&g_memory->dynamic_chunk, size);
	}
	else
	{
		if (size <= g_memory->tiny_chunk_size)
		{
			if ((ptr = allocate_static_chunk(g_memory->tiny_chunk, g_memory->tiny_chunk_size, size)) == NULL)
				ptr = allocate_dynamic_chunk(&g_memory->dynamic_chunk, size);
		}
		else if (size <= g_memory->medium_chunk_size)
		{
			g_memory->total_allocated_size += (size - chunk->allocated_size);
			chunk->allocated_size = size;
			return chunk->free_space;
		}
		else
			ptr = allocate_dynamic_chunk(&g_memory->dynamic_chunk, size);
	}
	chunk_cpy(chunk, ptr, size);

	return ptr;
}

/*
 * My own implementation of realloc using mmap/munmap.
 */
void		*realloc(void *ptr, size_t size)
{
	t_memory_chunk *chunk;

	pthread_mutex_lock(&g_mutex);
	if (is_allocated(ptr) == false)
		return NULL;
	chunk = (t_memory_chunk * )((char *) ptr - sizeof(t_memory_chunk));

	if (size != chunk->allocated_size)
	{
		if (chunk->allocation_type == STATIC_ALLOCATION)
			ptr = realloc_static_allocation(chunk, size);
		else
			ptr = realloc_dynamic_allocation(chunk, size);
	}
	pthread_mutex_unlock(&g_mutex);

	return ptr;
}
