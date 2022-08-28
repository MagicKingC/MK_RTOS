#ifndef MKRTOS_INC_H
#define MKRTOS_INC_H

#include <mktype.h>
#include <mkrtos_config.h>

typedef struct _MK_Task_TCB_ mk_TaskTcb;
typedef mk_TaskTcb _MK_LIST_NODE;

struct _MK_Task_TCB_{
	volatile mk_TaskStack *TaskStack;//线程栈
	char TaskName[MK_THREAD_NAME];//线程名
	mk_uint8  TaskPrio;//线程优先级
	mk_uint32 TaskDelayTicks;//任务定时时间
	
	struct _MK_Task_TCB_ *Prev;
	struct _MK_Task_TCB_ *Next;
};

MK_RTOS_EXT mk_uint32 _MK_Highest_Prio_Index;

MK_RTOS_EXT mk_TaskTcb *_MK_Current_Pro_;
MK_RTOS_EXT mk_TaskTcb *_MK_Highest_Pro_;

#endif
