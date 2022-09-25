#include <mkrtos.h>
#include <idle.h>

/* main���� */
static mk_TaskStack maintaskEnv[MAIN_TASK_STACK_SIZE];
mk_TaskTcb maintask;
void main_task(void *param){
	main();
	//��main�����˳�������ɾ��������
	mk_Task_Delete(MK_NULL);
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
	
	//main();
	mk_TaskInit("main",&maintask,(void *)main_task,MK_NULL,&maintaskEnv[512],MAIN_TASK_PRIORITY,1);
	
	_MK_Highest_Prio_Index = GetHighestPrioFromPrioTable();
	_MK_Highest_Pro_ = _MK_ReadyList[_MK_Highest_Prio_Index].Prev;

	//��ʼ����ϵͳ,���Ҵ��ж�
	_CPU_InterruptEnable_();
	_MK_RTOS_RUN_();
	
}


