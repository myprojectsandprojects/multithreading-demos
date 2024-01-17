#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include <pthread.h>

int Counter; // 0

pthread_mutex_t MutuallyExclusiveLock;

void *ThreadProc(void *Arg)
{
	printf("thead started\n");

	for(int i = 0; i < 1000000; ++i)
	{
		pthread_mutex_lock(&MutuallyExclusiveLock);
		Counter += 1;
		pthread_mutex_unlock(&MutuallyExclusiveLock);
	}

	return NULL;
}

int main()
{
	pthread_mutex_init(&MutuallyExclusiveLock, NULL);

	pthread_t Thread1Id;
	int Result = pthread_create(&Thread1Id, NULL, ThreadProc, NULL);
	assert(Result == 0);

	pthread_t Thread2Id;
	Result = pthread_create(&Thread2Id, NULL, ThreadProc, NULL);
	assert(Result == 0);

	Result = pthread_join(Thread1Id, NULL); assert(Result == 0);
	Result = pthread_join(Thread2Id, NULL); assert(Result == 0);

	pthread_mutex_destroy(&MutuallyExclusiveLock);

	printf("Counter: %d\n", Counter);

	return 0;
}
