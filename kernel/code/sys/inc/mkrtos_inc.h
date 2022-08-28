#ifndef MKRTOS_INC_H
#define MKRTOS_INC_H

#include <mktype.h>
#include <mkrtos_config.h>

typedef struct _MK_Task_TCB_ mk_TaskTcb;
typedef mk_TaskTcb _MK_LIST_NODE;

struct _MK_Task_TCB_{
	volatile mk_TaskStack *TaskStack;//�߳�ջ
	char TaskName[MK_THREAD_NAME];//�߳���
	mk_uint8  TaskPrio;//�߳����ȼ�
	mk_uint32 TaskDelayTicks;//����ʱʱ��
	
	struct _MK_Task_TCB_ *Prev;
	struct _MK_Task_TCB_ *Next;
};

MK_RTOS_EXT mk_uint32 _MK_Highest_Prio_Index;

MK_RTOS_EXT mk_TaskTcb *_MK_Current_Pro_;
MK_RTOS_EXT mk_TaskTcb *_MK_Highest_Pro_;

#endif
