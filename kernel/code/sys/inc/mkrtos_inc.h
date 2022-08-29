#ifndef MKRTOS_INC_H
#define MKRTOS_INC_H

#include <mktype.h>
#include <mkrtos_config.h>


typedef struct __MK_TICK_SPOKE__ MK_TICK_SPOKE;
typedef struct _MK_Task_TCB_ mk_TaskTcb;
typedef mk_TaskTcb _MK_LIST_NODE;

struct _MK_Task_TCB_{
	
	volatile mk_TaskStack *TaskStack;//�߳�ջ
	char TaskName[MK_THREAD_NAME];//�߳���
	mk_uint8  TaskPrio;//�߳����ȼ�
	
	//�����б�
	struct _MK_Task_TCB_ *ReadyPrev;
	struct _MK_Task_TCB_ *ReadyNext;
	
	//ʱ���б�
	struct _MK_Task_TCB_ *TickPrev;
	struct _MK_Task_TCB_ *TickNext;
	struct __MK_TICK_SPOKE__ *TickSpoke;
	mk_uint32 WaitTick;
	mk_uint32 TickCount;
	
	//ʱ��Ƭ
	mk_uint32 TaskTimeSlice;//ʣ��ʱ��Ƭ
	mk_uint32 TaskMaxTimeSlice;//��ʱ��Ƭ
	
};

struct __MK_TICK_SPOKE__{
	mk_TaskTcb *Next;
	mk_uint32 TickSpokeNodeNum;
	mk_uint32 TickSpokeNodeMax;
};


MK_RTOS_EXT mk_uint32 _MK_Highest_Prio_Index;

MK_RTOS_EXT mk_TaskTcb *_MK_Current_Pro_;
MK_RTOS_EXT mk_TaskTcb *_MK_Highest_Pro_;

#endif
