#include "ARMCM3.h"
#include "systick.h"
#include "switch.h"
#include "system.h"


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
	mk_uint8 i =0;
	for(i=0;i<2;i++){
		if(_OSReadyList[i].Head->TaskDelayTicks>0){
			_OSReadyList[i].Head->TaskDelayTicks--;
		}
	}
	_mk_task_switch_();
}


