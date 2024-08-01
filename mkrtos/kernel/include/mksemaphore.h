#ifndef MKSEMAPHORE_H
#define MKSEMAPHORE_H

#include <mkipc.h>

#if MK_USE_SEMAPHORE

/**
 * @brief 信号量
 */
typedef struct _MK_SEMAPHONE_ {
    mk_object_t obj;
    mk_bool_t is_use;
    mk_size_t value;
} mk_sem_t;

mk_code_t mk_sem_create(mk_sem_t* _sem, const char* _name, mk_size_t _sem_value);
mk_code_t mk_sem_release(mk_sem_t* _sem);
mk_code_t mk_sem_task(mk_sem_t* _sem, mk_size_t _tick_time);
void mk_printf_sem_list(mk_sem_t* _sem);

#endif

#endif