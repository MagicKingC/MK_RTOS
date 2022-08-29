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

