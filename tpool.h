#ifndef __TPOOL_H__
#define __TPOOL_H__

#define T TPool_T
typedef struct T *T;

T tpool_new(int value);
void tpool_destroy(T *self);
void tpool_add_work(T self, void (*func_p)(void *), void *arg_p);

#undef T
#endif
