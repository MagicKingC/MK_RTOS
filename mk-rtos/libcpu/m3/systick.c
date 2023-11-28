#include <mkrtos.h>
#include "ARMCM3.h"
#include "stm32f10x.h"


__weak void mk_SystickInit(mk_uint32 ms){
	//设置时间节拍
	SysTick->LOAD = (ms * SystemCoreClock/1000)-1;
	//配置优先级
	NVIC_SetPriority(SysTick_IRQn,(1<<__NVIC_PRIO_BITS) -1);
	//复位当前计数器的值
	SysTick->VAL = 0;
	//选择时钟源（内部时钟）,使能中断，异常使能
	SysTick->CTRL = 0x07
	
}

__weak void SysTick_Handler(void){
	
	mk_uint32 c_res = mk_critical_enter();
	UpdateToTickSpokeList();
#if USE_TIME_SLICE
	_MK_TaskTimeSliceSched(&_MK_ReadyList[_MK_Highest_Prio_Index]);
#endif
	mk_critical_exit(c_res);
	_MK_TaskSwitch_();

}


