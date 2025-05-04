#ifndef MKTASK_H
#define MKTASK_H

#include <mkinc.h>

#define MEM32(addr) *(__MK_IO32*)(addr)
#define MEM8(addr) *(__MK_IO8*)(addr)

/**
 * @brief 获取结构体变量偏移量
 */
#define GET_STR_DATA_ADDR_OFFSET(type, data) ((mk_size_t) & (((type*)0)->data))

extern mk_stack_t* g_next_task_sp;
extern mk_stack_t* g_current_task_sp;
extern mk_task_t* g_current_task;
extern mk_task_t idle_task;

MK_RTOS_EXT mk_code_t mk_task_init(const char* _task_name,
                                   mk_task_t* _task,
                                   void (*_entry)(void*),
                                   void* _param,
                                   mk_stack_t* _stack,
                                   mk_size_t _stack_size,
                                   mk_size_t _task_pro,
                                   mk_size_t _timer_tick);

MK_RTOS_EXT mk_code_t mk_task_start(mk_task_t* _task);
MK_RTOS_EXT mk_code_t mk_task_resume(mk_task_t* _task);
MK_RTOS_EXT mk_code_t mk_task_suspend(mk_task_t* _task);

MK_RTOS_EXT void mk_tack_scheduler(void);
MK_RTOS_EXT void mk_init_idle_task(void);

MK_RTOS_EXT mk_task_t* mk_get_current_task(void);
MK_RTOS_EXT mk_size_t mk_get_current_task_prio(void);
MK_RTOS_EXT char* mk_get_current_task_name(void);

MK_RTOS_EXT mk_code_t mk_task_tick_delay(mk_size_t _systick_times);
MK_RTOS_EXT void mk_task_delay_ms(mk_size_t _ms);

/**
 * @brief 汇编函数集
 */
MK_RTOS_EXT void mk_enable_interrupt(void);
MK_RTOS_EXT void mk_disable_interrupt(void);
MK_RTOS_EXT void mk_call_svc(void);
MK_RTOS_EXT mk_uint32_t mk_enter_critical(void);
MK_RTOS_EXT void mk_exit_critical(mk_uint32_t _status);
MK_RTOS_EXT void mk_trigger_pendsv();

#endif