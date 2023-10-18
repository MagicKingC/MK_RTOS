#include <mkrtos.h>
#include <idle.h>
#include <priority.h>

//任务切换
void _MK_TaskSwitch_(void){
	mk_uint32 c_res = mk_critical_enter();

	_MK_Highest_Prio_Index = GetHighestPrioFromPrioTable();
	_MK_Highest_Pro_ = _MK_ReadyList[_MK_Highest_Prio_Index].Prev;
	mk_critical_exit(c_res);
	_TriggerPendSV_();
}

/*******************************************************
时间片切换
1、判断为空闲任务或者没有下一个任务直接退出
2、判断时间片还有剩余时间片，直接退出
3、跳过1、2的判断，之后将重新赋值时间片，并且将其移动到优先级列表后面
********************************************************/
void _MK_TaskTimeSliceSched(MK_READY_LIST_NODE *list){
	
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