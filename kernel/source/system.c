#include "system.h"
#include "systick.h"
#include "idle.h"
#include "time_stamp.h"
#include "mkrtos_config.h"

void mk_task_init(mk_task_tcb * task,void (*entry)(void *),void *param,mk_taskstack *stack){
	
	*(--stack) = (mk_uint32)(1<<24);//必须置为1，否则进入arm模式，运行异常
	*(--stack) = (mk_uint32)(entry);
	//硬件自动保存
	*(--stack) = (mk_uint32)(0x14);
	*(--stack) = (mk_uint32)(0x12);
	*(--stack) = (mk_uint32)(0x3);
	*(--stack) = (mk_uint32)(0x2);
	*(--stack) = (mk_uint32)(0x1);
	*(--stack) = (mk_uint32)(param);
	//手动保存
	*(--stack) = (mk_uint32)(0x11);
	*(--stack) = (mk_uint32)(0x10);
	*(--stack) = (mk_uint32)(0x9);
	*(--stack) = (mk_uint32)(0x8);
	*(--stack) = (mk_uint32)(0x7);
	*(--stack) = (mk_uint32)(0x6);
	*(--stack) = (mk_uint32)(0x5);
	*(--stack) = (mk_uint32)(0x4);
	
	task->Stack = stack;
	
}

void mk_rtos_init(void){
	//关中断
	_CPU_InterruptDisable_();
	
	//初始化系统定时器
	mk_SystickInit(10);
	
#if USE_TIME_STAMP
	//初始化时间戳
	mk_time_stamp_init();
#endif
	
	//初始化空闲任务
	mk_idle_init();
	
	//开始运行系统,并且打开中断
	_MK_Next_Pro_ = _OSReadyList[0].Head;
	_MK_RTOS_RUN_();
	
}


