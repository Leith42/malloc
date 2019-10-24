#ifndef MALLOC_H
# define MALLOC_H

# define CHUNK_MAX			100
# define TINY_PAGE_MAX		8
# define MEDIUM_PAGE_MAX	32

# include <stdio.h>
# include <stdbool.h>

typedef	enum
{
	STATIC_ALLOCATION,
	DYNAMIC_ALLOCATION
}	t_allocation_type;


typedef struct	s_memory_chunk
{
	struct s_memory_chunk	*prev;
	struct s_memory_chunk	*next;
	void					*free_space;
	size_t					allocated_size;
	size_t					projection_size;
	t_allocation_type		allocation_type;
	bool					free;
}				t_memory_chunk;

typedef struct	s_memory
{
	t_memory_chunk	*tiny_chunk;
	t_memory_chunk	*medium_chunk;
	t_memory_chunk	*dynamic_chunk;
	size_t			system_page_size;
	size_t			tiny_chunk_size;
	size_t			medium_chunk_size;
	size_t			total_allocated_size;
	size_t			projection_size;
}				t_memory;

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	init_memory(void);
void	print_memory(const void *addr, size_t size);
void	*allocate_memory(size_t size);
void	show_alloc_mem(void);
void	print_address_hex(void* p0);
void 	*allocate_static_chunk(t_memory_chunk *chunk, size_t chunk_size, size_t allocated_size);
void	*allocate_dynamic_chunk(t_memory_chunk **chunk, size_t size);
// Miscellaneous
size_t	align_to_page_size(size_t size);

extern t_memory	*memory;

#endif //MALLOC_H
