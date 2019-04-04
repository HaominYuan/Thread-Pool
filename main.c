#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "squeue.h"

SQueue_T q;

void *producer(void *arg) {
	int i;
	for (i = 0; i < 10; i++) {
		squeue_enqueue(q, i);
		printf("Thread %s put number: %d\n", (char *) arg, i);
		sleep(rand() % 5);
	}
	return NULL;
}

void *consumer(void *arg) {
	int i, value, status;
	for (i = 0; i < 10; i++) {
		status = squeue_dequeue(q, &value);
		if (status) {
			printf("Queue is empty!\n");
		} else {
			printf("Thread %s get number: %d\n", (char *) arg, value);
		}
		sleep(rand() % 1);
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_t p1, p2;
	q = squeue_new();
	pthread_create(&p1, NULL, producer, "A");
	pthread_create(&p2, NULL, consumer, "B");
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	squeue_destroy(&q);
	return 0;
}
