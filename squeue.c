#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include "squeue.h"

#define T SQueue_T

typedef struct node_t {
	void*         value;
	struct node_t *next;
} node_t;

struct SQueue_T {
	node_t          *head;
	node_t          *tail;
	pthread_mutex_t headLock;
	pthread_mutex_t tailLock;
	pthread_cond_t  getCond;
	pthread_cond_t  putCond;
};

T squeue_new(void) {
	T self = malloc(sizeof(*self));
	node_t *tmp = malloc(sizeof(node_t));
	tmp->next = NULL;
	self->head = self->tail = tmp;
	pthread_mutex_init(&self->headLock, NULL);
	pthread_mutex_init(&self->tailLock, NULL);
	return self;
}

void squeue_destroy(T *self) {
	node_t *tmp;
	do {
		tmp = (*self)->head->next;
		free((*self)->head);
		(*self)->head = tmp;
	} while(tmp != NULL);
	free(*self);
	*self = NULL;
}

void squeue_enqueue(T self, void *value) {
	node_t *tmp = malloc(sizeof(node_t));
	tmp->value = value;
	tmp->next = NULL;

	pthread_mutex_lock(&self->tailLock);
	self->tail->next = tmp;
	self->tail = tmp;
	pthread_mutex_unlock(&self->tailLock);
}

int squeue_dequeue(T self, void **value) {
	pthread_mutex_lock(&self->headLock);	
	node_t *tmp = self->head;
	node_t *newHead = tmp->next;
	if (newHead == NULL) {
		pthread_mutex_unlock(&self->headLock);
		return -1;
	}
	*value = newHead->value;
	self->head = newHead;
	pthread_mutex_unlock(&self->headLock);
	free(tmp);
	return 0;
}
