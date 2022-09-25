#include "ARMCM3.h"
#include <mkrtos.h>

void mk_SystickInit(mk_uint32 ms){
	//����ʱ�����
	SysTick->LOAD = (ms * SystemCoreClock/1000)-1;
	//�������ȼ�
	NVIC_SetPriority(SysTick_IRQn,(1<<__NVIC_PRIO_BITS) -1);
	//��λ��ǰ��������ֵ
	SysTick->VAL = 0;
	//ѡ��ʱ��Դ,ʹ���жϺͼ�����
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk|SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk;
	
}

void SysTick_Handler(void){
	
	mk_uint32 c_res = mk_critical_enter();
	
	UpdateToTickSpokeList();
	
#if USE_TIME_SLICE
	_MK_TaskTimeSliceSched(&_MK_ReadyList[_MK_Highest_Prio_Index]);
#endif
	
	mk_critical_exit(c_res);
	
	_MK_TaskSwitch_();

}


