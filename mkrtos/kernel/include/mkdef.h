#ifndef MKDEF_H
#define MKDEF_H

#include "mkconfig.h"

#define MK_FALSE 0
#define MK_TRUE 1
#define MK_NULL (void*)0

/*************************************/
/**
 * 常用的系统数据类型
 */
typedef char mk_int8_t;
typedef short mk_int16_t;
typedef int mk_int32_t;

typedef unsigned char mk_uint8_t;
typedef unsigned short mk_uint16_t;
typedef unsigned int mk_uint32_t;

typedef mk_uint8_t mk_bool_t;

typedef long mk_base_t;
typedef unsigned long mk_ubase_t;
typedef mk_base_t mk_size_t;

#define __mk_weak __attributes__((weak))
#define __MK_IO volatile mk_base_t
#define __MK_IO8 volatile mk_uint8_t
#define __MK_IO32 volatile mk_uint32_t

/******************************************/

/******************************************/
#ifdef __GNUC__
#define _MK_ATTRIBUTE(attrs) __attribute__((attrs))
#else
#define _MK_ATTRIBUTE(attrs)
#endif

#define MK_ALIGN_DOWN(size, align) ((size) & ~((align)-1))

/**
 * 系统独有的数据类型
 */

typedef mk_uint32_t mk_stack_t;

/**
 * @brief 系统码
 */
enum _MK_CODE_ {
    MK_FAIL = -1,
    MK_SUCCESS = 0,
    MK_BUSY,
};

/**
 * @brief 任务状态
 */
enum _MK_TASK_STATUS_ {
    MK_TASK_STATUS_CTREATE = 0,  // 创建态
    MK_TASK_STATUS_READY,        // 就绪态
    MK_TASK_STATUS_SUSPEND,      // 挂起态
    MK_TASK_STATUS_RUNNING,      // 运行态
    MK_TASK_STATUS_COLOSE,       // 结束态
};

typedef struct _mk_task_tcb_ mk_task_t;
typedef enum _MK_CODE_ mk_code_t;
typedef enum _MK_TASK_STATUS_ mk_task_status_t;

typedef struct _MK_LIST {
    mk_task_t* prev;             /*上一个任务*/
    mk_task_t* next;             /*下一个任务*/
    mk_base_t task_num; /* 任务数 */
} mk_list_t;

typedef mk_list_t mk_ready_list_t;
typedef mk_list_t mk_delay_list_t;
typedef mk_list_t mk_sem_list_t;

struct _mk_task_tcb_ {
    char task_name[MK_TASK_NAME_LEN]; /* 任务名字 */

    mk_stack_t* task_sp;  /* 任务栈指针*/
    mk_size_t stack_size; /* 任务栈大小 */

    mk_size_t delay_systick; /* 系统滴答 */
    mk_size_t prio;          /* 优先级 */

    mk_size_t timer_tick;      /* 剩余的时间片 */
    mk_size_t base_timer_tick; /* 时间片 */

    mk_task_status_t task_status; /* 任务状态 */

    mk_ready_list_t ready_list; /* 就绪队列 */
    mk_delay_list_t delay_list; /* 延时队列 */
    mk_sem_list_t sem_list;     /* 信号量队列 */

} _MK_ATTRIBUTE(aligned(1));

#ifdef MK_USE_SOFTTIMER
/**
 * @brief 定时器节点
 */
typedef struct _mk_timer_node_ mk_timer_t;
typedef void (*mk_timer_callback_fun_t)(void *);

typedef enum MK_TIMER_FLAGE {
    MK_TIMER_ONCE = 0,
    MK_TIMER_REPEAT,
}mk_timer_flage_t;

struct _mk_timer_node_
{
    mk_timer_t *prev;
    mk_timer_t *next;
    void *para;
    mk_size_t base_systick;
    mk_size_t timerout_systick;
    mk_base_t flage;
    mk_timer_callback_fun_t callback_fun;
}_MK_ATTRIBUTE(aligned(1));

#endif

#ifdef MK_RTOS_GLOABLS
#define MK_RTOS_EXT extern
#else
#define MK_RTOS_EXT
#endif

MK_RTOS_EXT void __mk_assert_func(const char*, int, const char*, const char*) _MK_ATTRIBUTE(__noreturn__);
#define MK_ASSERT(_error, _msg) (_error) ? (void*)0 : (__mk_assert_func(__FILE__, __LINE__, __func__, #_msg))


#define mk_weak __attribute__((weak))


#endif
