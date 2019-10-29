#include <unistd.h>
#include <string.h>
#include "malloc.h"

void print(char *s)
{
	write(1, s, strlen(s));
}

int main()
{
	char *addr;
	
	addr = malloc(16);
	free(NULL);
	free((void *)addr);
	if (realloc((void *)addr + 5, 10) == NULL)
		print("Bonjours\n");
	return 0;
}
