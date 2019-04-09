#ifndef __ZEM_H__
#define __ZEM_H__

#define T Zem_T
typedef struct T *T;

T zem_new(int value);
void zem_destroy(T *self);
void zem_wait(T self);
void zem_post(T self);
void zem_com(T self);
void zem_check(T self);

#undef T
#endif
