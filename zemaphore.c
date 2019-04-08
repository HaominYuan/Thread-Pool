#include "zemaphore.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define T Zem_T
struct Zem_T {
	int value;
	pthread_cond_t cond;
	pthread_mutex_t lock;
};

T zem_new(int value) {
	T self = malloc(sizeof(*self));
	self->value = value;
	pthread_mutex_init(&self->lock, NULL);
	pthread_cond_init(&self->cond, NULL);
	return self;
}

void zem_destroy(T *self) {
	pthread_mutex_destroy(&(*self)->lock);
	pthread_cond_destroy(&(*self)->cond);
	free(*self);
	*self = NULL;
}

void zem_wait(T self) {
	pthread_mutex_lock(&self->lock);
	while (self->value <= 0) {
		pthread_cond_wait(&self->cond, &self->lock);
	}
	self->value = self->value - 1;
	pthread_mutex_unlock(&self->lock);
}

void zem_post(T self) {
	pthread_mutex_lock(&self->lock);
	self->value = self->value + 1;
	pthread_cond_signal(&self->cond);
	pthread_mutex_unlock(&self->lock);
}
