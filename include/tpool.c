#include "tpool.h"
#include "squeue.h"
#include "zem.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define T TPool_T
struct TPool_T {
	int npthreads;
	int nrunning;
	pthread_t *pthreads;
	SQueue_T queue;
	Zem_T zem;
};

typedef struct job_t {
	void (*func_p)(void *);
	void (*arg);
} job_t;

static void *run(void *arg) {	
	job_t *job;
	
	T self = (T) (arg);
	pthread_detach(pthread_self());
	
	while (true) {
		zem_wait(self->zem);
		squeue_dequeue(self->queue, (void **) &job);
		job->func_p(job->arg);
		free(job);
		zem_com(self->zem);
	}

	return NULL;
}

T tpool_new(int value) {
	int i;
	T self = malloc(sizeof(*self));

	/* 初始化正在运行的线程个数 */
	self->nrunning = 0;

	/* 初始化队列以及信号量 */
	self->queue = squeue_new();
	self->zem = zem_new(0);

	/* 初始化线程 */
	self->pthreads = malloc(value * sizeof(pthread_t));
	self->npthreads = value;
	for (i = 0; i < value; i++) {
		pthread_create(self->pthreads + i, NULL, run, self);
	}

	return self;
}

void tpool_destroy(T *self) { 
	zem_check((*self)->zem);
	free((*self)->pthreads);
	squeue_destroy(&(*self)->queue);
	zem_destroy(&(*self)->zem);
	free(*self);
	*self = NULL;
}

void tpool_add_work(T self, void (*func_p)(void *), void *arg) {
	job_t *job = malloc(sizeof(*job));
	job->func_p = func_p;
	job->arg = arg;
	squeue_enqueue(self->queue, (void *) job);
	zem_post(self->zem);
}
