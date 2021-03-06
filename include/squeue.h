#ifndef __SQUEUE_H__
#define __SQUEUE_H__

#define T SQueue_T
typedef struct T *T;

T squeue_new(void);
void squeue_destroy(T *self);
void squeue_enqueue(T self, void *value);
int squeue_dequeue(T self, void **value);

#undef T
#endif
