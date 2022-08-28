#include "ARMCM3.h"
#include "systick.h"
#include "switch.h"
#include "system.h"
#include "idle.h"
#include <priority.h>


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
	mk_uint32 index = 0;
	mk_uint32 c_res = mk_critical_enter();
	for(index = 0;index < MK_PRIORITY_MAX; index++)
	{
		if(_MK_ReadyList[index].Prev){
			if(_MK_ReadyList[index].Prev->TaskDelayTicks > 0){
				_MK_ReadyList[index].Prev->TaskDelayTicks--;
				if(_MK_ReadyList[index].Prev->TaskDelayTicks == 0){
					SetBitToPrioTable(_MK_ReadyList[index].Prev->TaskPrio);
				}
			}
		}
	}
	mk_critical_exit(c_res);
	_mk_task_switch_();

}


