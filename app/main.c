#include "mkrtos.h"


//创建任务
mk_TaskStack taskEnv1[512];
mk_TaskStack taskEnv2[512];
mk_TaskStack taskEnv3[512];

mk_TaskTcb task_1;
mk_TaskTcb task_2;
mk_TaskTcb task_3;

int bit1 = 0;
volatile int bit2 = 0;
volatile int bit3 = 0;

void delay(mk_uint32 count )
{
 for (; count!=0; count--);
}


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
		//mk_delay_ms(2);
		delay(0xff);
		bit2=1;
		//mk_delay_ms(2);
		delay(0xff);
	}
}

void task3(void *param){
	while(1){
		bit3=0;
		//mk_delay_ms(2);
		delay(0xff);
		bit3=1;
		//mk_delay_ms(2);
		delay(0xff);
	}
}


int main(void){
	
	mk_TaskInit("task1",&task_1,task1,MK_NULL,&taskEnv1[512],1,0);
	mk_TaskInit("task2",&task_2,task2,MK_NULL,&taskEnv2[512],2,1);
	mk_TaskInit("task3",&task_3,task3,MK_NULL,&taskEnv3[512],2,1);
	
	return 0;
}

