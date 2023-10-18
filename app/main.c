// #include <mkrtos.h>


// //��������
// mk_TaskStack taskEnv1[512];
// mk_TaskStack taskEnv2[512];
// mk_TaskStack taskEnv3[512];

// mk_TaskTcb task_1;
// mk_TaskTcb task_2;
// mk_TaskTcb task_3;

// int bit1 = 0;
// volatile int bit2 = 0;
// volatile int bit3 = 0;

// void delay(mk_uint32 count )
// {
//  for (; count!=0; count--);
// }


// void task1(void *param){
// 	while(1){
// 		bit1=0;
// 		mk_delay_ms(2);
// 		bit1=1;
// 		mk_delay_ms(2);
// 	}
// }

// void task2(void *param){
// 	while(1){
// 		bit2=0;
// 		delay(0xff);
// 		bit2=1;
// 		delay(0xff);
// 	}
// }

// void task3(void *param){
// 	while(1){
// 		bit3=0;
// 		delay(0xff);
// 		bit3=1;
// 		delay(0xff);
// 	}
// }

// mk_task_t task1_s={0};
// mk_task_t task2_s={0};
// mk_task_t task3_s={0};

// int main(void){
	
// 	task1_s.is_auto_stck  = MK_FALSE;
// 	task1_s.task_prio  = 1;
// 	task1_s.task_stack = &taskEnv1[512];
// 	task1_s.task_time_slice = 0;
// 	task1_s.param = MK_NULL;
// 	task1_s.task_entry = task1;
// 	task1_s.task_name = "task1";
	
// 	task2_s.is_auto_stck  = MK_FALSE;
// 	task2_s.task_prio  = 2;
// 	task2_s.task_stack = &taskEnv2[512];
// 	task2_s.task_time_slice = 1;
// 	task2_s.param = MK_NULL;
// 	task2_s.task_entry = task2;
// 	task2_s.task_name = "task2";
	
// 	task3_s.is_auto_stck  = MK_FALSE;
// 	task3_s.task_prio  = 2;
// 	task3_s.task_stack = &taskEnv3[512];
// 	task3_s.task_time_slice = 1;
// 	task3_s.param = MK_NULL;
// 	task3_s.task_entry = task3;
// 	task3_s.task_name = "task3";
	
	
// 	mk_Task_Init(&task2_s);
// 	mk_Task_Init(&task3_s);
// 	mk_Task_Init(&task1_s);
	

// //	mk_TaskInit("task2",&task_2,task2,MK_NULL,&taskEnv2[512],2,1);
// //	mk_TaskInit("task3",&task_3,task3,MK_NULL,&taskEnv3[512],2,1);
// //	mk_TaskInit("task1",&task_1,task1,MK_NULL,&taskEnv1[512],1,0);
	
// 	return 0;
// }

#include "cm3.h"
#include "mkrtos.h"

extern mk_uint32_t _bss;
extern mk_uint32_t _ebss;


int main()
{


    mk_printk("Hello RTOS\n");
    mk_printk("psp:0x%x\n", get_psp());
    mk_printk("msp:0x%x\n", get_msp());

    while(1) {

    }

    return 0;
}
