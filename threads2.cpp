#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include <pthread.h>
#include <sys/sysinfo.h>

pthread_mutex_t MutuallyExclusiveLock;

void *ThreadProc(void *Arg)
{
	printf("thread %lu started\n", (uint64_t)Arg);

	for(uint32_t i = 0; i < 0xffffffff; ++i);

	printf("thread %lu finished\n", (uint64_t)Arg);

	return NULL;
}

int main()
{
	int NumCores = get_nprocs();
	printf("num cores: %d\n", NumCores);
	const int NumThreads = NumCores - 1; // + main thread (?) @should set some max probably
	pthread_t Threads[NumThreads];

	pthread_mutex_init(&MutuallyExclusiveLock, NULL);

	for(int ThreadIndex = 0; ThreadIndex < NumThreads; ++ThreadIndex)
	{
		int Result = pthread_create(&Threads[ThreadIndex], NULL, ThreadProc, (void *)ThreadIndex);
		assert(Result == 0);
	}

	for(int ThreadIndex = 0; ThreadIndex < NumThreads; ++ThreadIndex)
	{
		int Result = pthread_join(Threads[ThreadIndex], NULL); assert(Result == 0);
	}
	printf("all threads finished\n");

	pthread_mutex_destroy(&MutuallyExclusiveLock);

	return 0;
}
