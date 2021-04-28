#include "ARMCM3.h"
#include "systick.h"
#include "switch.h"


void mk_SystickInit(mk_uint32 ms){
	//设置时间节拍
	SysTick->LOAD = (ms * SystemCoreClock/1000)-1;
	//配置优先级
	NVIC_SetPriority(SysTick_IRQn,(1<<__NVIC_PRIO_BITS) -1);
	//复位当前计数器的值
	SysTick->VAL = 0;
	//选择时钟源,使能中断和计数器
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;
	
}

void SysTick_Handler(void){
	_mk_task_switch_();
}


