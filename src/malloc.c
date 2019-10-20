#include "malloc.h"
#include "libft.h"
#include <errno.h>
#include <sys/mman.h>

void *malloc(size_t size) {
	void *allocation;

	if (size == 0)
		return (NULL);
	init_memory();
	allocation = allocate_memory(size);

	if (memory == MAP_FAILED)
		return (NULL);
	return allocation;
}
