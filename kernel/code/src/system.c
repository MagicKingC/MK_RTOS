#include <mkrtos.h>
#include <idle.h>

/* main函数 */
static mk_TaskStack maintaskEnv[MAIN_TASK_STACK_SIZE];
mk_TaskTcb maintask;
void main_task(void *param){
	main();
	//从main函数退出，主动删除该任务
	mk_Task_Delete(MK_NULL);
}

void __MK_Main(void){
	//关中断
	_CPU_InterruptDisable_();
	
	//初始化系统定时器
	mk_SystickInit(1);
	
#if USE_TIME_STAMP
	//初始化时间戳
	mk_time_stamp_init();
#endif
	_MK_Current_Pro_ = MK_NULL;
	_MK_Highest_Pro_ = MK_NULL;
	
	//初始化优先级表
	InitPrioTable();
	
	//初始化就绪列表
	InitReadyList();
	
	//初始化时基列表
	InitTickSpokeList();
	
	//初始化空闲任务
	_MK_Idle_Init_();
	
	//main();
	mk_TaskInit("main",&maintask,(void *)main_task,MK_NULL,&maintaskEnv[512],MAIN_TASK_PRIORITY,1);
	
	_MK_Highest_Prio_Index = GetHighestPrioFromPrioTable();
	_MK_Highest_Pro_ = _MK_ReadyList[_MK_Highest_Prio_Index].Prev;

	//开始运行系统,并且打开中断
	_CPU_InterruptEnable_();
	_MK_RTOS_RUN_();
	
}


