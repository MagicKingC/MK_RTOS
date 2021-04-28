#ifndef MKRTOS_INC_H
#define MKRTOS_INC_H

#include "data_type.h"
typedef struct _MK_Task_ mk_stack;
struct _MK_Task_{
	mk_taskstack *stack;
};

typedef struct os_ready_list OS_READY_LIST;//就绪链表
struct os_ready_list{
	mk_stack	*Head;
	mk_stack	*Next;
};

MK_RTOS_EXT OS_READY_LIST _OSReadyList[10];//就绪列表

MK_RTOS_EXT mk_stack *_MK_Current_Pro_;
MK_RTOS_EXT mk_stack *_MK_Next_Pro_;

#endif
