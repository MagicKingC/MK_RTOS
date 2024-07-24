#ifndef MKLIST_H
#define MKLIST_H

#include <mkinc.h>

MK_RTOS_EXT void mk_init_task_bit_map(void);
MK_RTOS_EXT void mk_set_bitmap(mk_base_t _index);
MK_RTOS_EXT void mk_clear_bitmap(mk_base_t _index);
MK_RTOS_EXT mk_base_t mk_find_hight_priority();

// 系统表
MK_RTOS_EXT void mk_init_system_list();

// 就绪列表
MK_RTOS_EXT mk_code_t mk_insert_node_to_ready_list(mk_task_t* _task);
MK_RTOS_EXT mk_task_t* mk_get_node_from_ready_list(mk_size_t _highest_pro_index);
MK_RTOS_EXT mk_code_t mk_delete_node_from_ready_list(mk_task_t* _task);
MK_RTOS_EXT void mk_printf_ready_list(mk_size_t _prio);

// 延迟列表
MK_RTOS_EXT mk_code_t mk_insert_node_to_delay_list(mk_task_t* _task);
MK_RTOS_EXT mk_code_t mk_delete_node_from_delay_list(mk_task_t* _task);
MK_RTOS_EXT void mk_update_delay_list(void);
MK_RTOS_EXT void mk_printf_delay_list(void);

// 列表基本操作
MK_RTOS_EXT void mk_init_list(mk_list_t* _list);
MK_RTOS_EXT mk_code_t mk_insert_node_to_list_tail(mk_list_t* _list, mk_task_t* _task, mk_size_t _offset);
MK_RTOS_EXT mk_code_t mk_delete_node_from_list(mk_list_t* _list, mk_task_t* _task, mk_size_t _offset);
MK_RTOS_EXT mk_task_t* mk_get_node_from_list(mk_list_t* _list);


#endif
