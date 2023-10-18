#include <mktask.h>
#include <system.h>
#include <priority.h>
#include <mkstr.h>
#include <scheduler.h>


/******************************************************************
*	寄存器说明
*	PC寄存器:指向的时cpu即将执行的地址
*	LR寄存器：存储子程序跳转地址
*	R3、R2、R1、R0：子程序之间传递数据
*	R4到R11用于存储局部变量
*	硬件自动将xPSP寄存器、R15(PC)、LR(R14)、R12、R3、R2、R1、R0压入栈
*******************************************************************/

static mk_TaskStack * __MK_TaskStackInit(void (*_entry)(void *),void *_param,mk_TaskStack *_TaskStack){

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

/********************************************
*   静态方法创建任务
*	TaskName:		线程名字
*	TaskTCB:		程序控制块
*	_entry:			程序入口
*	_param:			传入程序参数
*	_TaskStack:		堆栈地址
*	_TaskPrio:		任务优先级
*	_TaskTimeSlice:	时间片
**********************************************/

mk_code_t mk_TaskInit(char * TaskName, mk_TaskTcb * TaskTCB, void (*_entry)(void *), void *_param, 
					mk_TaskStack *_TaskStack, mk_uint32 _TaskPrio ,mk_uint32 _TaskTimeSlice){

	mk_uint32 c_res = mk_critical_enter();
	if(TaskTCB == MK_NULL || _entry == MK_NULL || _TaskStack == MK_NULL){
		mk_critical_exit(c_res);
		return MK_FAIL;
	}
	if(_TaskPrio > MK_PRIORITY_MAX ){
		return MK_FAIL;
	}
	//时基参数
	TaskTCB->WaitTick = 0;
	TaskTCB->TickCount = 0;
	TaskTCB->TickPrev = MK_NULL;
	TaskTCB->TickNext = MK_NULL;
	
#if USE_TIME_SLICE
	//时间片参数
	TaskTCB->TaskTimeSlice = _TaskTimeSlice;
	TaskTCB->TaskMaxTimeSlice = _TaskTimeSlice;
#endif
	//设置优先级
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
					
mk_code_t mk_Task_Init(mk_task_t *_task_struct){
	mk_uint32 c_res = mk_critical_enter();
	if(_task_struct->task_entry == MK_NULL){
		mk_critical_exit(c_res);
		return MK_FAIL;
	}
	//判断是静态栈还是动态栈
	if(_task_struct->is_auto_stck){
		//_task_struct->task_stack = malloc()
	}
	else
	{
		if(_task_struct->task_stack == MK_NULL){
			mk_critical_exit(c_res);
			return MK_FAIL;
		}
	}
	
	mk_int32 str_len = mk_strlen(_task_struct->task_tcb.TaskName);
	mk_int32 task_name_len = str_len > MK_TASK_NAME_LEN ? MK_TASK_NAME_LEN : str_len;
	
	mk_strncpy(_task_struct->task_tcb.TaskName,_task_struct->task_name,task_name_len);
	
	if(_task_struct->task_prio > MK_PRIORITY_MAX ){
		mk_critical_exit(c_res);
		return MK_FAIL;
	}
	//时基参数
	_task_struct->task_tcb.WaitTick = 0;
	_task_struct->task_tcb.TickCount = 0;
	_task_struct->task_tcb.TickPrev = MK_NULL;
	_task_struct->task_tcb.TickNext = MK_NULL;
	
#if USE_TIME_SLICE
	//时间片参数
	_task_struct->task_tcb.TaskTimeSlice = _task_struct->task_time_slice;
	_task_struct->task_tcb.TaskMaxTimeSlice = _task_struct->task_time_slice;
#endif
	//设置优先级
	_task_struct->task_tcb.TaskPrio = _task_struct->task_prio;
	
	//初始化栈
	_task_struct->task_tcb.TaskStack =
							__MK_TaskStackInit(_task_struct->task_entry,
											   _task_struct->param,
											   _task_struct->task_stack);
	
	//插入就绪列表
	InsertNodeToReadyListTail(&(_task_struct->task_tcb));
	//设置优先级
	SetBitToPrioTable(_task_struct->task_tcb.TaskPrio);
	
	mk_critical_exit(c_res);
	
	return MK_SUCCESS;
}

/******************************************
*   mk_Task_Delete
*    功能：任务删除函数
*	参数：线程函数块
******************************************/
void mk_Task_Delete(mk_TaskTcb *_task){
	mk_uint32 c_res = mk_critical_enter();
	if(_task == MK_NULL){
		RemoveNodeFromReadyList(_MK_Current_Pro_);
	}else{
		RemoveNodeFromReadyList(_task);
	}
	mk_critical_exit(c_res);
	_MK_TaskSwitch_();
}