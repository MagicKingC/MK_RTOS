#include "system.h"

void _mk_task_switch_(void){
	if(_MK_Current_Pro_ == _OSReadyList[0].Head )
	{
		_MK_Next_Pro_ = _OSReadyList[1].Head;
	}
	else
	{
		_MK_Next_Pro_ = _OSReadyList[0].Head;
	}
	_TriggerPendSV_();

}

