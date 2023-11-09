#include "cm3.h"
#include "mkrtos.h"

extern mk_uint32_t _bss;
extern mk_uint32_t _ebss;

mk_TaskStack taskEnv1[512];
mk_TaskStack taskEnv2[512];
mk_TaskStack taskEnv3[512];

mk_TaskTcb task_1;
mk_TaskTcb task_2;
mk_TaskTcb task_3;

int bit1 = 0;
volatile int bit2 = 0;
volatile int bit3 = 0;

void delay(mk_uint32_t count )
{
 for (; count!=0; count--);
}

void task1(void *param){
	while(1){
		mk_printk("%s\n",__func__);
		mk_delay_ms(1000);
	}
}

void task2(void *param){
	while(1){
		mk_printk("%s\n",__func__);
		delay(0xFFFF);
	}
}

void task3(void *param){
	while(1){
		mk_printk("%s\n",__func__);
		delay(0xFFFF);
	}
}

mk_task_t task1_s={0};
mk_task_t task2_s={0};
mk_task_t task3_s={0};

int main()
{
    mk_printk("Hello RTOS\n");
    mk_printk("psp:0x%x\n", get_psp());
    mk_printk("msp:0x%x\n", get_msp());

    mk_TaskInit("task2",&task_2,task2,MK_NULL,&taskEnv2[512],2,1);
    mk_TaskInit("task3",&task_3,task3,MK_NULL,&taskEnv3[512],2,1);
    mk_TaskInit("task1",&task_1,task1,MK_NULL,&taskEnv1[512],1,1);

    while(1) {
        mk_printk("%s\n",__func__);
        mk_delay_ms(50);
    }

    return 0;
}
	