#ifndef MKSEMAPHORE_H
#define MKSEMAPHORE_H

#include <mkinc.h>

#if MK_USE_SEMAPHORE

typedef struct _MK_COM_ {
    char name[MK_COM_NAME_LEN];
    mk_list_t sem_list;
    mk_bool_t is_use;
} mk_com_t;

/**
 * @brief 信号量
 */
typedef struct _MK_SEMAPHONE_ {
    mk_com_t obj;
    mk_size_t value;
} mk_sem_t;

mk_code_t mk_sem_create(mk_sem_t *_sem, const char *_name, mk_size_t _sem_value);
mk_code_t mk_sem_release(mk_sem_t *_sem);
mk_code_t mk_sem_get(mk_sem_t *_sem);
void mk_printf_sem_list(mk_sem_t *_sem);

#endif

#endif