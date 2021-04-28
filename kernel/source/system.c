#include "system.h"
#include "systick.h"
#include "idle.h"

void mk_task_init(mk_task_tcb * task,void (*entry)(void *),void *param,mk_taskstack *stack){
	
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
	
	task->Stack = stack;
	
}

void mk_rtos_init(void){
	//���ж�
	_CPU_InterruptDisable_();
	//��ʼ��ϵͳ��ʱ��
	mk_SystickInit(10);
	//��ʼ����������
	mk_idle_init();
	//��ʼ����ϵͳ,���Ҵ��ж�
	_MK_Next_Pro_ = _OSReadyList[0].Head;
	_MK_RTOS_RUN_();
	
}


