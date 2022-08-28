#include <system.h>
#include <systick.h>
#include <idle.h>
#include <mkrtos_config.h>
#include <priority.h>
#include <delay.h>

static mk_TaskStack * __MK_TaskStackInit(void (*_entry)(void *),void *_param,mk_TaskStack *_TaskStack){
	//�Ĵ���˵��
	//PC�Ĵ���:ָ���ʱcpu����ִ�еĵ�ַ
	//LR�Ĵ������洢�ӳ�����ת��ַ
	//R3��R2��R1��R0���ӳ���֮�䴫������
	//R4��R11���ڴ洢�ֲ�����
	//Ӳ���Զ���xPSP�Ĵ�����R15(PC)��LR(R14)��R12��R3��R2��R1��R0ѹ��ջ
	*(--_TaskStack) = (mk_uint32)(1<<24);//������Ϊ1���������armģʽ�������쳣
	*(--_TaskStack) = (mk_uint32)(_entry);//R15
	//Ӳ���Զ�����
	*(--_TaskStack) = (mk_uint32)(0x00000014ul);//R14
	*(--_TaskStack) = (mk_uint32)(0x00000012ul);//R12
	*(--_TaskStack) = (mk_uint32)(0x00000003ul);//R3
	*(--_TaskStack) = (mk_uint32)(0x00000002ul);//R2
	*(--_TaskStack) = (mk_uint32)(0x00000001ul);//R1
	*(--_TaskStack) = (mk_uint32)(_param);//R0
	//�ֶ�����
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
TaskName:�߳�����
TaskTCB:������ƿ�
_entry:�������
_param:����������
_TaskStack:��ջ��ַ
_TaskPrio:�������ȼ�
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
	//��ʼ��ջ
	TaskTCB->TaskStack = __MK_TaskStackInit(_entry,_param,_TaskStack);
	//��������б�
	InsertNodeToReadyListTail(TaskTCB);
	//�������ȼ�
	SetBitToPrioTable(TaskTCB->TaskPrio);
	
	mk_critical_exit(c_res);
	return MK_SUCCESS;
	
}

void __MK_Main(void){
	//���ж�
	_CPU_InterruptDisable_();
	
	//��ʼ��ϵͳ��ʱ��
	mk_SystickInit(1);
	
#if USE_TIME_STAMP
	//��ʼ��ʱ���
	mk_time_stamp_init();
#endif
	_MK_Current_Pro_ = MK_NULL;
	_MK_Highest_Pro_ = MK_NULL;
	
	//��ʼ�����ȼ���
	InitPrioTable();
	
	//��ʼ�������б�
	InitReadyList();
	
	//��ʼ��ʱ���б�
	InitTickSpokeList();
	
	//��ʼ����������
	_MK_Idle_Init_();
	main();
	_MK_Highest_Prio_Index = GetHighestPrioFromPrioTable();
	_MK_Highest_Pro_ = _MK_ReadyList[_MK_Highest_Prio_Index].Prev;

	//��ʼ����ϵͳ,���Ҵ��ж�
	_CPU_InterruptEnable_();
	_MK_RTOS_RUN_();
	
}


