#include "mkrtos.h"


//创建任务
mk_TaskStack taskEnv1[512];
mk_TaskStack taskEnv2[512];
mk_TaskStack taskEnv3[512];

mk_TaskTcb task_1;
mk_TaskTcb task_2;
mk_TaskTcb task_3;

int bit1 = 0;
int bit2 = 0;
int bit3 = 0;

void task1(void *param){
	while(1){
		bit1=0;
		mk_delay_ms(2);
		bit1=1;
		mk_delay_ms(2);
	}
}

void task2(void *param){
	while(1){
		bit2=0;
		mk_delay_ms(2);
		bit2=1;
		mk_delay_ms(2);
	}
}

void task3(void *param){
	while(1){
		bit3=0;
		mk_delay_ms(2);
		bit3=1;
		mk_delay_ms(2);
	}
}


int main(void){
	
	mk_TaskInit("task1",&task_1,task1,&taskEnv1[512],1,(void *)0);
	mk_TaskInit("task2",&task_2,task2,&taskEnv2[512],2,(void *)0);
	mk_TaskInit("task3",&task_3,task3,&taskEnv3[512],3,(void *)0);
	
	return 0;
}

