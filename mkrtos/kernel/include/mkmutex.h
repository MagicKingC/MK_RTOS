#ifndef MKMUTEX_H
#define MKMUTEX_H 

#include <mkipc.h>


#if MK_USE_MUTEX

typedef enum {
    MK_MUTEX_UNLOCK = 0,
    MK_MUTEX_LOCK
}mk_mutex_status_t;

/**
 * @brief 信号量
 */
typedef struct _MK_MUTEX_ {
    mk_object_t obj;
    mk_bool_t is_use;
    mk_task_t* owner;
    mk_mutex_status_t status;
    mk_size_t owner_prio;
} mk_mutex_t;

mk_code_t mk_mutex_create(mk_mutex_t* _mutex, const char* _name);
mk_code_t mk_mutex_unlock(mk_mutex_t* _mutex);
mk_code_t mk_mutex_lock(mk_mutex_t* _mutex, mk_ticks_t _tick_time);
void mk_printf_mutex_list(mk_mutex_t* _mutex);

#endif


#endif