#include "system.h"
#include "systick.h"
#include "idle.h"
#include "time_stamp.h"
#include "mkrtos_config.h"
#include <priority.h>

/*
task:������ƿ�
entry:�������
param:����������
stack:��ջ��ַ
TaskLevel:�������ȼ�
*/
void mk_task_init(mk_task_tcb * TaskTCB,void (*entry)(void *),void *param,mk_TaskStack *stack,mk_uint8 TaskLevel){
	//�Ĵ���˵��
	//PC�Ĵ���:ָ���ʱcpu����ִ�еĵ�ַ
	//LR�Ĵ������洢�ӳ�����ת��ַ
	//R3��R2��R1��R0���ӳ���֮�䴫������
	//R4��R11���ڴ洢�ֲ�����
	//Ӳ���Զ���xPSP�Ĵ�����R15(PC)��LR(R14)��R12��R3��R2��R1��R0ѹ��ջ
	*(--stack) = (mk_uint32)(1<<24);//������Ϊ1���������armģʽ�������쳣
	*(--stack) = (mk_uint32)(entry);
	//Ӳ���Զ�����
	*(--stack) = (mk_uint32)(0x14);
	*(--stack) = (mk_uint32)(0x12);
	*(--stack) = (mk_uint32)(0x3);
	*(--stack) = (mk_uint32)(0x2);
	*(--stack) = (mk_uint32)(0x1);
	*(--stack) = (mk_uint32)(param);
	//�ֶ�����
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
	
	//�������ȼ���
	_MK_InsertTCBToPrioTable(TaskTCB);
	
}

int _MK_main_(void){
	mk_rtos_init();
	return 0;
}

void mk_rtos_init(void){
	//���ж�
	_CPU_InterruptDisable_();
	
	//��ʼ��ϵͳ��ʱ��
	mk_SystickInit(10);
	
#if USE_TIME_STAMP
	//��ʼ��ʱ���
	mk_time_stamp_init();
#endif
	
	//��ʼ����������
	_MK_Idle_Init_();
	
	//��ʼ�����ȼ���
	_MK_PrioInit_();

	//��ʼ����ϵͳ,���Ҵ��ж�
	
//	_MK_Next_Pro_ = _OSReadyList[0].Head;
	_MK_Next_Pro_ = _MK_GetTCBFromPrioTable();
	_MK_RTOS_RUN_();
	
}


