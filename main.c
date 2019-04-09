#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "squeue.h"
#include "zem.h"
#include "tpool.h"

#define N 5

void task1() {
	sleep(rand() % 1);
	printf("Thread #%u working on task1\n", (int) pthread_self());
}

void task2() {
	sleep(rand() % 1);
	printf("Thread #%u working on task2\n", (int) pthread_self());
}

int main(int argc, char *argv[]) {
	int i;
	TPool_T tp = tpool_new(4);
	for (i = 0; i < 5; i++) {
		tpool_add_work(tp, task1, NULL);
		tpool_add_work(tp, task2, NULL);
	}
	tpool_destroy(&tp);
	return 0;
}
