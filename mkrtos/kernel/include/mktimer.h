#ifndef MKTIMER_H
#define MKTIMER_H

#ifdef MK_USE_SOFTTIMER

#include <mkinc.h>

MK_RTOS_EXT void mk_init_soft_timer_list(void);
MK_RTOS_EXT mk_timer_t mk_timer_create(const char* _name,
                                       mk_timer_callback_fun_t _callback_fun,
                                       void* _para,
                                       mk_size_t _timer_systick,
                                       mk_timer_flage_t _flage);

MK_RTOS_EXT void mk_init_timer_task(void);
MK_RTOS_EXT mk_code_t mk_start_timer(mk_timer_t* _timer);
MK_RTOS_EXT mk_code_t mk_updte_timer_list(void);
MK_RTOS_EXT mk_code_t mk_insert_node_to_soft_timer_list(mk_timer_t* _node);


#endif

#endif
