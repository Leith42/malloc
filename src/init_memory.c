#include "malloc.h"
#include <sys/mman.h>
#include <unistd.h>
#include "libft.h"

/*
 * Mutex
 */
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

/*
 * Main structure, contain everything needed.
 */
t_memory *g_memory = NULL;

/*
 * Initializes the first memory chunk of each type,
 * other nodes will be dynamically initialized when needed.
 */
static void init_static_chunks(void) {
	g_memory->tiny_chunk = (t_memory_chunk * )(g_memory + 1);
	g_memory->tiny_chunk->prev = NULL;
	g_memory->tiny_chunk->next = NULL;
	g_memory->tiny_chunk->free_space = (char *) g_memory->tiny_chunk + sizeof(t_memory_chunk);
	g_memory->tiny_chunk->allocated_size = 0;
	g_memory->tiny_chunk->projection_size = sizeof(t_memory_chunk) + g_memory->tiny_chunk_size;
	g_memory->tiny_chunk->allocation_type = STATIC_ALLOCATION;
	g_memory->tiny_chunk->free = true;
	g_memory->medium_chunk = (t_memory_chunk * )(
			(char *) g_memory->tiny_chunk + ((g_memory->tiny_chunk_size + sizeof(t_memory_chunk)) * CHUNK_MAX));
	g_memory->medium_chunk->prev = NULL;
	g_memory->medium_chunk->next = NULL;
	g_memory->medium_chunk->free_space = (char *) g_memory->medium_chunk + sizeof(t_memory_chunk);
	g_memory->medium_chunk->allocated_size = 0;
	g_memory->medium_chunk->projection_size = sizeof(t_memory_chunk) + g_memory->medium_chunk_size;
	g_memory->medium_chunk->allocation_type = STATIC_ALLOCATION;
	g_memory->medium_chunk->free = true;
	g_memory->dynamic_chunk = NULL;
}

/*
 * Initializes the main structure that will contain the chunks of memory and useful data.
 */
int	init_memory(void) {
	size_t system_page_size;
	size_t tiny_chunk_size;
	size_t medium_chunk_size;
	size_t projection_size;

	if (g_memory != NULL)
		return 0;
	system_page_size = getpagesize();
	tiny_chunk_size = system_page_size * TINY_PAGE_MAX;
	medium_chunk_size = system_page_size * MEDIUM_PAGE_MAX;
	projection_size = align_to_page_size(sizeof(t_memory)
										 + (tiny_chunk_size + sizeof(t_memory_chunk)) * CHUNK_MAX
										 + (medium_chunk_size + sizeof(t_memory_chunk)) * CHUNK_MAX);
	g_memory = mmap(NULL, projection_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (g_memory == MAP_FAILED)
		return -1;

	g_memory->system_page_size = system_page_size;
	g_memory->tiny_chunk_size = tiny_chunk_size;
	g_memory->medium_chunk_size = medium_chunk_size;
	g_memory->projection_size = projection_size;
	init_static_chunks();
	return 0;
}
