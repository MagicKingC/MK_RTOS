#ifndef MKRTOS_INC_H
#define MKRTOS_INC_H

#include <mktype.h>
#include <mkrtos_config.h>


typedef struct __MK_TICK_SPOKE__ MK_TICK_SPOKE;
typedef struct _MK_Task_TCB_ mk_TaskTcb;
typedef mk_TaskTcb _MK_LIST_NODE;

struct _MK_Task_TCB_{
	
	volatile mk_TaskStack *TaskStack;//线程栈
	char TaskName[MK_THREAD_NAME];//线程名
	mk_uint8  TaskPrio;//线程优先级
	
	//就绪列表
	struct _MK_Task_TCB_ *ReadyPrev;
	struct _MK_Task_TCB_ *ReadyNext;
	
	//时基列表
	struct _MK_Task_TCB_ *TickPrev;
	struct _MK_Task_TCB_ *TickNext;
	struct __MK_TICK_SPOKE__ *TickSpoke;
	mk_uint32 WaitTick;
	mk_uint32 TickCount;
	
	//时间片
	mk_uint32 TaskTimeSlice;//剩余时间片
	mk_uint32 TaskMaxTimeSlice;//总时间片
	
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
