#include "malloc.h"
#include "libft.h"

static void 	chunk_cpy(t_memory_chunk *chunk, void *dest, size_t size)
{
	if (chunk != NULL)
	{
		ft_memcpy(dest, chunk->free_space, chunk->allocated_size > size ? size : chunk->allocated_size);
		free(chunk->free_space);
	}
}

static void		*realloc_dynamic_allocation(t_memory_chunk *chunk, size_t size)
{
	void *ptr;

	if (size <= memory->tiny_chunk_size)
	{
		if ((ptr = allocate_static_chunk(memory->tiny_chunk, memory->tiny_chunk_size, size)) == NULL)
			ptr = allocate_dynamic_chunk(&memory->dynamic_chunk, size);
	}
	else if (size <= memory->medium_chunk_size)
	{
		if ((ptr = allocate_static_chunk(memory->medium_chunk, memory->medium_chunk_size, size)) == NULL)
			ptr = allocate_dynamic_chunk(&memory->dynamic_chunk, size);
	}
	else
	{
		ptr = allocate_dynamic_chunk(&memory->dynamic_chunk, size);
	}
	chunk_cpy(chunk, ptr, size);

	return (ptr);
}

static void		*realloc_static_allocation(t_memory_chunk *chunk, size_t size)
{
	void *ptr;

	if (chunk < memory->medium_chunk)
	{
		if (size <= memory->tiny_chunk_size)
		{
			memory->total_allocated_size += (size - chunk->allocated_size);
			chunk->allocated_size = size;
			return chunk->free_space;
		}
		else if (size <= memory->medium_chunk_size)
		{
			if ((ptr = allocate_static_chunk(memory->medium_chunk, memory->medium_chunk_size, size)) == NULL)
				ptr = allocate_dynamic_chunk(&memory->dynamic_chunk, size);
		}
		else
			ptr = allocate_dynamic_chunk(&memory->dynamic_chunk, size);
	}
	else
	{
		if (size <= memory->tiny_chunk_size)
		{
			if ((ptr = allocate_static_chunk(memory->tiny_chunk, memory->tiny_chunk_size, size)) == NULL)
				ptr = allocate_dynamic_chunk(&memory->dynamic_chunk, size);
		}
		else if (size <= memory->medium_chunk_size)
		{
			memory->total_allocated_size += (size - chunk->allocated_size);
			chunk->allocated_size = size;
			return chunk->free_space;
		}
		else
			ptr = allocate_dynamic_chunk(&memory->dynamic_chunk, size);
	}
	chunk_cpy(chunk, ptr, size);

	return ptr;
}

void		*realloc(void *ptr, size_t size)
{
	t_memory_chunk *chunk;

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

	return ptr;
}
