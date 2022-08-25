#include "system.h"
#include "systick.h"
#include "idle.h"
#include "time_stamp.h"
#include "mkrtos_config.h"
#include <priority.h>

/*
task:程序控制块
entry:程序入口
param:传入程序参数
stack:堆栈地址
TaskLevel:任务优先级
*/
void mk_task_init(mk_task_tcb * TaskTCB,void (*entry)(void *),void *param,mk_TaskStack *stack,mk_uint8 TaskLevel){
	//寄存器说明
	//PC寄存器:指向的时cpu即将执行的地址
	//LR寄存器：存储子程序跳转地址
	//R3、R2、R1、R0：子程序之间传递数据
	//R4到R11用于存储局部变量
	//硬件自动将xPSP寄存器、R15(PC)、LR(R14)、R12、R3、R2、R1、R0压入栈
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
	
	TaskTCB->TaskLevel = TaskLevel > MK_PRIORITY_MAX ? MK_PRIORITY_MAX:TaskLevel;
	TaskTCB->Stack = stack;
	
	//插入优先级表
	_MK_InsertTCBToPrioTable(TaskTCB);
	
}

int _MK_main_(void){
	mk_rtos_init();
	return 0;
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
	_MK_Idle_Init_();
	
	//初始化优先级表
	_MK_PrioInit_();

	//开始运行系统,并且打开中断
	
//	_MK_Next_Pro_ = _OSReadyList[0].Head;
	_MK_Next_Pro_ = _MK_GetTCBFromPrioTable();
	_MK_RTOS_RUN_();
	
}


