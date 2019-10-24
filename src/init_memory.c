#include "malloc.h"
#include <sys/mman.h>
#include <unistd.h>
#include "libft.h"

/*
 * Main structure, contain everything needed.
 */
t_memory *memory = NULL;

/*
 * Initializes the first memory chunk of each type,
 * other nodes will be dynamically initialized when needed.
 */
static void init_static_chunks(void) {
	memory->tiny_chunk = (t_memory_chunk * )(memory + 1);
	memory->tiny_chunk->prev = NULL;
	memory->tiny_chunk->next = NULL;
	memory->tiny_chunk->free_space = (char *) memory->tiny_chunk + sizeof(t_memory_chunk);
	memory->tiny_chunk->allocated_size = 0;
	memory->tiny_chunk->projection_size = sizeof(t_memory_chunk) + memory->tiny_chunk_size;
	memory->tiny_chunk->allocation_type = STATIC_ALLOCATION;
	memory->tiny_chunk->free = true;
	memory->medium_chunk = (t_memory_chunk * )(
			(char *) memory->tiny_chunk + ((memory->tiny_chunk_size + sizeof(t_memory_chunk)) * CHUNK_MAX));
	memory->medium_chunk->prev = NULL;
	memory->medium_chunk->next = NULL;
	memory->medium_chunk->free_space = (char *) memory->medium_chunk + sizeof(t_memory_chunk);
	memory->medium_chunk->allocated_size = 0;
	memory->medium_chunk->projection_size = sizeof(t_memory_chunk) + memory->medium_chunk_size;
	memory->medium_chunk->allocation_type = STATIC_ALLOCATION;
	memory->medium_chunk->free = true;
	memory->dynamic_chunk = NULL;
}

/*
 * Initializes the main structure that will contain the chunks of memory and useful data.
 */
void init_memory(void) {
	size_t system_page_size;
	size_t tiny_chunk_size;
	size_t medium_chunk_size;
	size_t projection_size;

	if (memory != NULL)
		return;
	system_page_size = getpagesize();
	tiny_chunk_size = system_page_size * TINY_PAGE_MAX;
	medium_chunk_size = system_page_size * MEDIUM_PAGE_MAX;
	projection_size = align_to_page_size(sizeof(t_memory)
										 + (tiny_chunk_size + sizeof(t_memory_chunk)) * CHUNK_MAX
										 + (medium_chunk_size + sizeof(t_memory_chunk)) * CHUNK_MAX);
	memory = mmap(NULL, projection_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (memory == MAP_FAILED)
		return;

	memory->system_page_size = system_page_size;
	memory->tiny_chunk_size = tiny_chunk_size;
	memory->medium_chunk_size = medium_chunk_size;
	memory->projection_size = projection_size;
	init_static_chunks();
}
