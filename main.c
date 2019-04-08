#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "squeue.h"
#include "zemaphore.h"

SQueue_T q;
Zem_T z;

void *producer(void *arg) {
	int i, tmp;
	int *sum = malloc(sizeof(int));
	*sum = 0;
	for (i = 0; i < 10; i++) {
		tmp = rand() % 5;
		squeue_enqueue(q, tmp);
		*sum = *sum + tmp;
		zem_post(z);
		sleep(rand() % 1);
	}
	return sum;
}

void *consumer(void *arg) {
	int i, value;
	int *sum = malloc(sizeof(int));
	*sum = 0;
	for (i = 0; i < 10; i++) {
		zem_wait(z);
		squeue_dequeue(q, &value);
		*sum = *sum + value;
		sleep(rand() % 1);
	}
	return sum;
}

int main(int argc, char *argv[]) {
	pthread_t p1, p2, p3, p4;
	int *r1, *r2, *r3, *r4;
	q = squeue_new();
	z = zem_new(0);
	pthread_create(&p1, NULL, producer, "A");
	pthread_create(&p2, NULL, producer, "B");
	pthread_create(&p3, NULL, consumer, "c");
	pthread_create(&p4, NULL, consumer, "d");
	pthread_join(p1, (void **) &r1);
	pthread_join(p2, (void **) &r2);
	pthread_join(p3, (void **) &r3);
	pthread_join(p4, (void **) &r4);
	printf("producer sum: %d\n", *r1 + *r2);
	printf("consumer sum: %d\n", *r3 + *r4);
	squeue_destroy(&q);
	zem_destroy(&z);
	return 0;
}
