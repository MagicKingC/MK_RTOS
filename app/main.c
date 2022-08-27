#include "mkrtos.h"


//创建任务
mk_TaskStack taskEnv1[512];
mk_TaskStack taskEnv2[512];

mk_task_tcb task_1;
mk_task_tcb task_2;
void task1(void *param);
void task2(void *param);

int _MK_main_(void){
	_MK_DelayInit();
	_MK_PrioInit_();
	
	mk_task_init(&task_1,task1,(void *)1,&taskEnv1[512],1);
	mk_task_init(&task_2,task2,(void *)2,&taskEnv2[512],2);
	
	mk_rtos_init();
	return 0;
}

int main(void){
	

	
//	_OSReadyList[0].Head = &task_1;
//	_OSReadyList[1].Head = &task_2;
	
	return 0;
}

int bit1 = 0;
int bit2 = 0;

void task1(void *param){
	while(1){
		bit1 = 0;
		mk_delay_ms(1);
		bit1 = 1;
		mk_delay_ms(1);
	}

}

void task2(void *param){
	while(1){
		bit2 = 0;
		mk_delay_ms(1);
		bit2 = 1;
		mk_delay_ms(1);
		
	}

}
