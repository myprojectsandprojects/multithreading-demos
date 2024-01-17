#include <stdio.h>
#include <assert.h>
#include <stdint.h>

#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

uint64_t SafeSignedToUnsigned(int64_t Signed) {
    assert(Signed > -1);
    return Signed;
}

int64_t SafeUnsignedToSigned(uint64_t Unsigned) {
	uint64_t FirstUnsignedToBecomeNegative = 0x8000000000000000;
    assert(Unsigned < FirstUnsignedToBecomeNegative);
    return Unsigned;
}

/*
- read/write memory barriers:
	- for compiler
	- for CPU
	- also 'volatile' keyword
- interlocked increment
- semaphore, WaitForSingleObject()

- pthread_cond_wait()
*/

const int MaxNumTasks = 64;
int *Tasks[MaxNumTasks];
int NumTasks; // 0

int NumTaken; // 0
int NumDone; // 0

pthread_mutex_t lock;

sem_t Semaphore;


void AddTask(int *Task) {
	assert(NumTasks < MaxNumTasks);

	Tasks[NumTasks] = Task;
	NumTasks += 1;

	sem_post(&Semaphore);
}

struct threadInfo {
	int Index;
	pthread_t ThreadId;
};

void *ThreadProc(void *Arg) {
	threadInfo *Info = (threadInfo *)Arg;
	printf("thread %d started\n", Info->Index);

	while(true) {
		// pthread_mutex_lock(&lock);

		sem_wait(&Semaphore);

		int *Task = Tasks[NumTaken];
		NumTaken += 1;
		// pthread_mutex_unlock(&lock);

		// printf("[thread %d] %d squared: %d\n", Info->Index, Task, Task*Task);
		*Task *= *Task;

		// pthread_mutex_lock(&lock);
		NumDone += 1;
		// pthread_mutex_unlock(&lock);
	}

	return NULL;
}

const int NumThreads = 16;
threadInfo ThreadInfos[NumThreads];

int main() {
	sem_init(&Semaphore, 0, 0);

	// assert(pthread_mutex_init(&lock, NULL) == 0);

	for(int i = 0; i < NumThreads; ++i) {
		ThreadInfos[i].Index = i;

	    int Result = pthread_create(
	        &ThreadInfos[i].ThreadId, //?
	        NULL,              // attr
	        ThreadProc,
	        (void *)&ThreadInfos[i]);
	    assert(Result == 0);
	}

 //    sleep(6);

    // printf("waiting for thread to exit...\n");
    // void *Ret;
    // pthread_join(ThreadId, &Ret);
    // printf("thread exited with %lx\n", (uint64_t)Ret);

    int Numbers[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    int64_t NumbersNumElements = SafeUnsignedToSigned(sizeof(Numbers) / sizeof(Numbers[0]));

    for(int i = 0; i < NumbersNumElements; ++i) {
        // Numbers[i] *= Numbers[i];
        AddTask(&Numbers[i]);

        // printf("added %d as a task\n", Numbers[i]);
    }

    while(NumDone < NumTasks);

    for(int i = 0; i < NumbersNumElements; ++i) {
        printf("%d: %d\n", i, Numbers[i]);
    }

    return 0;
}
