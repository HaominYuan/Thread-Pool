#include "zem.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define T Zem_T
struct Zem_T {
	int value;
	int sum;
	bool can;
	pthread_mutex_t lock;
	pthread_cond_t com;
	pthread_cond_t cond;
};

T zem_new(int value) {
	T self = malloc(sizeof(*self));
	self->value = value;
	self->sum = 0;
	pthread_mutex_init(&self->lock, NULL);
	pthread_cond_init(&self->cond, NULL);
	pthread_cond_init(&self->com, NULL);
	return self;
}
void zem_destroy(T *self) {
	pthread_mutex_destroy(&(*self)->lock);
	pthread_cond_destroy(&(*self)->cond);
	pthread_cond_destroy(&(*self)->com);
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
	self->sum = self->sum = 1;
	pthread_cond_signal(&self->cond);
	pthread_mutex_unlock(&self->lock);
}

void zem_com(T self) {
	pthread_mutex_lock(&self->lock);
	self->sum = self->sum - 1;
	if (self->sum == 0) {
		pthread_cond_signal(&self->com);
	}
	pthread_mutex_unlock(&self->lock);
}

void zem_check(T self) {
	pthread_mutex_lock(&self->lock);
	while (self->sum > 0) {
		pthread_cond_wait(&self->com, &self->lock);
	}
	pthread_mutex_unlock(&self->lock);
}
