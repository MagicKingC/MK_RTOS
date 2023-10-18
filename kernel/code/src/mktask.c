#include <mktask.h>
#include <system.h>
#include <priority.h>
#include <mkstr.h>
#include <scheduler.h>


/******************************************************************
*	�Ĵ���˵��
*	PC�Ĵ���:ָ���ʱcpu����ִ�еĵ�ַ
*	LR�Ĵ������洢�ӳ�����ת��ַ
*	R3��R2��R1��R0���ӳ���֮�䴫������
*	R4��R11���ڴ洢�ֲ�����
*	Ӳ���Զ���xPSP�Ĵ�����R15(PC)��LR(R14)��R12��R3��R2��R1��R0ѹ��ջ
*******************************************************************/

/**
 * @brief ����ջ��ʼ��
 * @param _entry 
 * @param _param 
 * @param _TaskStack 
 * @return mk_TaskStack* 
 */
static mk_TaskStack * __MK_TaskStackInit(void (*_entry)(void *),void *_param,mk_TaskStack *_TaskStack){

	*(--_TaskStack) = (mk_uint32_t)(1<<24);//������Ϊ1���������armģʽ�������쳣
	*(--_TaskStack) = (mk_uint32_t)(_entry);//R15
	//Ӳ���Զ�����
	*(--_TaskStack) = (mk_uint32_t)(0x00000014ul);//R14
	*(--_TaskStack) = (mk_uint32_t)(0x00000012ul);//R12
	*(--_TaskStack) = (mk_uint32_t)(0x00000003ul);//R3
	*(--_TaskStack) = (mk_uint32_t)(0x00000002ul);//R2
	*(--_TaskStack) = (mk_uint32_t)(0x00000001ul);//R1
	*(--_TaskStack) = (mk_uint32_t)(_param);//R0
	//�ֶ�����
	*(--_TaskStack) = (mk_uint32_t)(0x00000011ul);//R11
	*(--_TaskStack) = (mk_uint32_t)(0x00000010ul);//R10
	*(--_TaskStack) = (mk_uint32_t)(0x00000009ul);//R9
	*(--_TaskStack) = (mk_uint32_t)(0x00000008ul);//R8
	*(--_TaskStack) = (mk_uint32_t)(0x00000007ul);//R7
	*(--_TaskStack) = (mk_uint32_t)(0x00000006ul);//R6
	*(--_TaskStack) = (mk_uint32_t)(0x00000005ul);//R5
	*(--_TaskStack) = (mk_uint32_t)(0x00000004ul);//R4
	
	return _TaskStack;
	
}


/**
 * @brief ��̬������������
 * @param TaskName �߳�����
 * @param TaskTCB  ������ƿ�
 * @param _entry �������
 * @param _param ����������
 * @param _TaskStack ��ջ��ַ
 * @param _TaskPrio �������ȼ�
 * @param _TaskTimeSlice ʱ��Ƭ
 * @return mk_code_t 
 */
mk_code_t mk_TaskInit(char * TaskName, mk_TaskTcb * TaskTCB, void (*_entry)(void *), void *_param, 
					mk_TaskStack *_TaskStack, mk_uint32_t _TaskPrio ,mk_uint32_t _TaskTimeSlice){

	mk_uint32_t c_res = mk_critical_enter();
	if(TaskTCB == MK_NULL || _entry == MK_NULL || _TaskStack == MK_NULL){
		mk_critical_exit(c_res);
		return MK_FAIL;
	}
	if(_TaskPrio > MK_PRIORITY_MAX ){
		return MK_FAIL;
	}
	//ʱ������
	TaskTCB->WaitTick = 0;
	TaskTCB->TickCount = 0;
	TaskTCB->TickPrev = MK_NULL;
	TaskTCB->TickNext = MK_NULL;
	
#if USE_TIME_SLICE
	//ʱ��Ƭ����
	TaskTCB->TaskTimeSlice = _TaskTimeSlice;
	TaskTCB->TaskMaxTimeSlice = _TaskTimeSlice;
#endif
	//�������ȼ�
	TaskTCB->TaskPrio = _TaskPrio;
	//��ʼ��ջ
	TaskTCB->TaskStack = __MK_TaskStackInit(_entry,_param,_TaskStack);
	//��������б�
	InsertNodeToReadyListTail(TaskTCB);
	//�������ȼ�
	SetBitToPrioTable(TaskTCB->TaskPrio);
	
	mk_critical_exit(c_res);
	
	return MK_SUCCESS;
	
}

/**
 * @brief �����ʼ��
 * @param _task_struct 
 * @return mk_code_t 
 */
mk_code_t mk_Task_Init(mk_task_t *_task_struct){
	mk_uint32_t c_res = mk_critical_enter();
	if(_task_struct->task_entry == MK_NULL){
		mk_critical_exit(c_res);
		return MK_FAIL;
	}
	//�ж��Ǿ�̬ջ���Ƕ�̬ջ
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
	
	mk_int32_t str_len = mk_strlen(_task_struct->task_tcb.TaskName);
	mk_int32_t task_name_len = str_len > MK_TASK_NAME_LEN ? MK_TASK_NAME_LEN : str_len;
	
	mk_strncpy(_task_struct->task_tcb.TaskName,_task_struct->task_name,task_name_len);
	
	if(_task_struct->task_prio > MK_PRIORITY_MAX ){
		mk_critical_exit(c_res);
		return MK_FAIL;
	}
	//ʱ������
	_task_struct->task_tcb.WaitTick = 0;
	_task_struct->task_tcb.TickCount = 0;
	_task_struct->task_tcb.TickPrev = MK_NULL;
	_task_struct->task_tcb.TickNext = MK_NULL;
	
#if USE_TIME_SLICE
	//ʱ��Ƭ����
	_task_struct->task_tcb.TaskTimeSlice = _task_struct->task_time_slice;
	_task_struct->task_tcb.TaskMaxTimeSlice = _task_struct->task_time_slice;
#endif
	//�������ȼ�
	_task_struct->task_tcb.TaskPrio = _task_struct->task_prio;
	
	//��ʼ��ջ
	_task_struct->task_tcb.TaskStack =
							__MK_TaskStackInit(_task_struct->task_entry,
											   _task_struct->param,
											   _task_struct->task_stack);
	
	//��������б�
	InsertNodeToReadyListTail(&(_task_struct->task_tcb));
	//�������ȼ�
	SetBitToPrioTable(_task_struct->task_tcb.TaskPrio);
	
	mk_critical_exit(c_res);
	
	return MK_SUCCESS;
}

/**
 * @brief ����ɾ��
 * @param _task ������
 */
void mk_Task_Delete(mk_TaskTcb *_task){
	mk_uint32_t c_res = mk_critical_enter();
	if(_task == MK_NULL){
		RemoveNodeFromReadyList(_MK_Current_Pro_);
	}else{
		RemoveNodeFromReadyList(_task);
	}
	mk_critical_exit(c_res);
	_MK_TaskSwitch_();
}