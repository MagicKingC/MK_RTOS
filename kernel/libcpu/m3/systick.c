#include "ARMCM3.h"
#include "systick.h"
#include "switch.h"
#include "system.h"
#include <idle.h>
#include <priority.h>
#include <tick_spoke.h>


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


//ʱ��Ƭ�л�
static void _MK_TaskTimeSliceSched(MK_READY_LIST_NODE *list){
	
	if(!list->Prev || list->Prev == &idletask)
		return;
	
	if(list->Prev->TaskTimeSlice > 0){
		list->Prev->TaskTimeSlice--;
	}
	
	if(list->Prev->TaskTimeSlice > 0 || list->TaskNum < 2){
		return;
	}

	list->Prev->TaskTimeSlice = list->Prev->TaskMaxTimeSlice;
	MoveHeadToTailInReadList(list);
	
	
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


