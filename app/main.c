#include "system.h"



void delay(){
	int i,j;
	for(i=0;i<30;i++){
		for(j=0;j<80;j++){
		}
	}
}

//创建任务
mk_taskstack taskEnv1[512];
mk_taskstack taskEnv2[512];

mk_stack task_1;
mk_stack task_2;
void task1(void *param);
void task2(void *param);


int main(void){
	
	mk_taskinit(&task_1,task1,(void *)1,&taskEnv1[512]);
	mk_taskinit(&task_2,task2,(void *)2,&taskEnv2[512]);
	
	_MK_Next_Pro_ = &task_1;
	_MK_RTOS_RUN_();
	return 0;
}

int bit1 = 0;
int bit2 = 0;

void task1(void *param){
	
	while(1){
		bit1 = 0;
		delay();
		bit1 = 1;
		delay();
		_MK_Next_Pro_ = &task_2;
		_TriggerPendSV_();
	}

}

void task2(void *param){
	while(1){
		bit2 = 0;
		delay();
		bit2 = 1;
		delay();
		_MK_Next_Pro_ = &task_1;
		_TriggerPendSV_();
	}

}
