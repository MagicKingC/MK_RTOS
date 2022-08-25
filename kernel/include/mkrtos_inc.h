#ifndef MKRTOS_INC_H
#define MKRTOS_INC_H

#include "data_type.h"
#include <mkrtos_config.h>

typedef struct _MK_Task_ mk_task_tcb;

struct _MK_Task_{
	mk_TaskStack *Stack;//必须放在结构体第一个位置，方便将地址赋值给堆栈指针
	mk_uint8  TaskLevel;//线程优先级
	mk_uint32 TaskDelayTicks;//任务定时时间
	mk_task_tcb	*Head;
	mk_task_tcb	*Next;
};

//就绪链表
//列表节点
//typedef struct os_list_node{
//	mk_task_tcb	*data;
//	struct os_list_node	*Head;
//	struct os_list_node	*Next;
//}_OS_LIST_NODE;
//列表
MK_RTOS_EXT mk_task_tcb _OSReadyList[MK_PRIORITY_MAX];

MK_RTOS_EXT mk_task_tcb *_MK_Current_Pro_;
MK_RTOS_EXT mk_task_tcb *_MK_Next_Pro_;

#endif
