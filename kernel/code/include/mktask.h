#ifndef RT_TASK_H
#define RT_TASK_H

#include <mktype.h>
#include <mkrtos_config.h>

//任务状态
#define MK_TASK_STATE_BIT_DLY			0x01u //延迟/超时位
#define MK_TASK_STATE_BIT_PEND			0x02u //等待位
#define MK_TASK_STATE_BIT_SUSPENDED		0x04u //挂起位

#define MK_TASK_STATY_RDY						0u //就绪
#define MK_TASK_STATY_DLY						1u //延时/超时
#define MK_TASK_STATY_PEND						2u //等待
#define MK_TASK_STATY__PEND_TIMEOUT				3u //等待+超时
#define MK_TASK_STATY_SUSPEND					4u //挂起
#define MK_TASK_STATY_DLY_SUSPEND				5u //挂起+延时/超时
#define MK_TASK_STATY_PEND_SUSPENDED			6u //挂起+等待
#define MK_TASK_STATY_PEND_TIMEOUT_SUSPENDED	7u //挂起+等待+超时
#define MK_TASK_STATY_DEL						255u //删除

typedef struct __MK_TICK_SPOKE__ MK_TICK_SPOKE;
typedef struct _MK_Task_TCB_ mk_TaskTcb;
typedef mk_TaskTcb _MK_LIST_NODE;

struct _MK_Task_TCB_{
	
	volatile mk_TaskStack *TaskStack;//线程栈
	char TaskName[MK_TASK_NAME_LEN];//线程名
	mk_TaskPrio  TaskPrio;//线程优先级
	
	//就绪列表
	struct _MK_Task_TCB_ *ReadyPrev;
	struct _MK_Task_TCB_ *ReadyNext;
	
	//时基列表
	struct _MK_Task_TCB_ *TickPrev;
	struct _MK_Task_TCB_ *TickNext;
	struct __MK_TICK_SPOKE__ *TickSpoke;
	mk_TaskTick WaitTick;
	mk_TaskTick TickCount;
	
	//时间片
	mk_TaskTime TaskTimeSlice;	/*剩余时间片*/
	mk_TaskTime TaskMaxTimeSlice;/*总时间片*/
	
	
	mk_TaskStatus TaskStatus;/*任务状态*/
	
	//挂起次数
	
};

/* 时间片列表 */
struct __MK_TICK_SPOKE__{
	mk_TaskTcb *Next;
	mk_uint32_t TickSpokeNodeNum;
	mk_uint32_t TickSpokeNodeMax;
};

/* 就绪优先级列表 */
typedef struct _MK_READY_LIST_NODE_ MK_READY_LIST_NODE;
struct _MK_READY_LIST_NODE_{
	mk_TaskTcb *Prev;
	mk_TaskTcb *Next;
	mk_uint32_t TaskNum;
};


MK_RTOS_EXT mk_uint32_t _MK_Highest_Prio_Index;/* 最高任务优先级的选项 */

MK_RTOS_EXT mk_TaskTcb *_MK_Current_Pro_;/* 当前任务块 */
MK_RTOS_EXT mk_TaskTcb *_MK_Highest_Pro_;/* 最高优先级任务 */

typedef struct _MK_TASK_{
	mk_TaskTcb task_tcb;		/* 任务控制块 */
	mk_TaskStack *task_stack;	/* 任务堆栈 */
	mk_TaskPrio task_prio;		/* 任务优先级 */
	mk_TaskTime task_time_slice;/* 任务时间片 */
	mk_bool_t 	is_auto_stck;	/* 是否自动分配任务栈 MK_FALSE:不自动分配 MK_TRUE:自动分配 */
	mk_uint32_t	task_stck_size; /* 是否任务栈大小，如果采用的是静态堆栈，该字段不必设置 */
	char *task_name;			/* 任务名字，最大长度由mkrtos_config.h的MK_TASK_NAME_LEN决定 */
	void (*task_entry)(void *); /* 任务入口函数 */
	void * param;				/* 任务参数 */
}mk_task_t;

/*任务初始化*/
//旧版任务初始化
MK_RTOS_EXT mk_code_t mk_TaskInit(char * TaskName, mk_TaskTcb * TaskTCB, void (*_entry)(void *), void *_param,\
					mk_TaskStack *_TaskStack, mk_uint32_t _TaskPrio ,mk_uint32_t _TaskTimeSlice);

MK_RTOS_EXT mk_code_t mk_Task_Init(mk_task_t *_task_struct);

MK_RTOS_EXT void mk_Task_Delete(mk_TaskTcb *_task);

#endif
