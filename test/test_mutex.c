#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "malloc.h"

pthread_t tid[3];
pthread_mutex_t lock;

void *doSomeThing(void *arg)
{
	malloc(1);
	return NULL;
}

int main(void) {
	int i = 0;
	int err;

	if (pthread_mutex_init(&lock, NULL) != 0)
	{
		printf("\n mutex init failed\n");
		return 1;
	}

	while (i < 3) {
		err = pthread_create(&(tid[i]), NULL, &doSomeThing, NULL);
		if (err != 0)
			printf("\ncan't create thread :[%s]", strerror(err));
		i++;
	}

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	pthread_join(tid[2], NULL);
	pthread_mutex_destroy(&lock);

	return 0;
}

