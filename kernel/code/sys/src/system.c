#include <system.h>
#include <systick.h>
#include <idle.h>
#include <mkrtos_config.h>
#include <priority.h>
#include <delay.h>

static mk_TaskStack * __MK_TaskStackInit(void (*_entry)(void *),void *_param,mk_TaskStack *_TaskStack){
	//寄存器说明
	//PC寄存器:指向的时cpu即将执行的地址
	//LR寄存器：存储子程序跳转地址
	//R3、R2、R1、R0：子程序之间传递数据
	//R4到R11用于存储局部变量
	//硬件自动将xPSP寄存器、R15(PC)、LR(R14)、R12、R3、R2、R1、R0压入栈
	*(--_TaskStack) = (mk_uint32)(1<<24);//必须置为1，否则进入arm模式，运行异常
	*(--_TaskStack) = (mk_uint32)(_entry);//R15
	//硬件自动保存
	*(--_TaskStack) = (mk_uint32)(0x00000014ul);//R14
	*(--_TaskStack) = (mk_uint32)(0x00000012ul);//R12
	*(--_TaskStack) = (mk_uint32)(0x00000003ul);//R3
	*(--_TaskStack) = (mk_uint32)(0x00000002ul);//R2
	*(--_TaskStack) = (mk_uint32)(0x00000001ul);//R1
	*(--_TaskStack) = (mk_uint32)(_param);//R0
	//手动保存
	*(--_TaskStack) = (mk_uint32)(0x00000011ul);//R11
	*(--_TaskStack) = (mk_uint32)(0x00000010ul);//R10
	*(--_TaskStack) = (mk_uint32)(0x00000009ul);//R9
	*(--_TaskStack) = (mk_uint32)(0x00000008ul);//R8
	*(--_TaskStack) = (mk_uint32)(0x00000007ul);//R7
	*(--_TaskStack) = (mk_uint32)(0x00000006ul);//R6
	*(--_TaskStack) = (mk_uint32)(0x00000005ul);//R5
	*(--_TaskStack) = (mk_uint32)(0x00000004ul);//R4
	
	return _TaskStack;
	
}

/*
TaskName:线程名字
TaskTCB:程序控制块
_entry:程序入口
_param:传入程序参数
_TaskStack:堆栈地址
_TaskPrio:任务优先级
*/
mk_code mk_TaskInit(char*TaskName,mk_TaskTcb * TaskTCB,void (*_entry)(void *),mk_TaskStack *_TaskStack,mk_uint8 _TaskPrio,void *_param){

	mk_uint32 c_res = mk_critical_enter();
	if(TaskTCB == MK_NULL || _entry == MK_NULL || _TaskStack == MK_NULL){
		mk_critical_exit(c_res);
		return MK_FAIL;
	}
	if(_TaskPrio > MK_PRIORITY_MAX ){
		return MK_FAIL;
	}
	TaskTCB->TaskDelayTicks = 0;
	TaskTCB->TaskPrio = _TaskPrio;
	//初始化栈
	TaskTCB->TaskStack = __MK_TaskStackInit(_entry,_param,_TaskStack);
	//插入就绪列表
	InsertNodeToReadyListTail(TaskTCB);
	//设置优先级
	SetBitToPrioTable(TaskTCB->TaskPrio);
	
	mk_critical_exit(c_res);
	return MK_SUCCESS;
	
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
	
	//初始化延迟列表
	InitDelayList();
	
	//初始化空闲任务
	_MK_Idle_Init_();
	main();
	_MK_Highest_Prio_Index = GetHighestPrioFromPrioTable();
	_MK_Highest_Pro_ = _MK_ReadyList[_MK_Highest_Prio_Index].Prev;

	//开始运行系统,并且打开中断
	_CPU_InterruptEnable_();
	_MK_RTOS_RUN_();
	
}


