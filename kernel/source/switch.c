#include "system.h"
#include "idle.h"

void _mk_task_switch_(void){
#if 0
	if(_MK_Current_Pro_ == _OSReadyList[0].Head )
	{
		_MK_Next_Pro_ = _OSReadyList[1].Head;
	}
	else
	{
		_MK_Next_Pro_ = _OSReadyList[0].Head;
	}
#endif
	if(_MK_Current_Pro_ == &idletask){
		if(_OSReadyList[0].Head->TaskDelayTicks == 0){
			_MK_Next_Pro_ = _OSReadyList[0].Head;
		}else if(_OSReadyList[1].Head->TaskDelayTicks == 0){
			_MK_Next_Pro_ = _OSReadyList[0].Head;
		}else{
			return;
		}
	}else{
		if(_MK_Current_Pro_ == _OSReadyList[0].Head){
			if(_OSReadyList[1].Head->TaskDelayTicks == 0){
				_MK_Next_Pro_ = _OSReadyList[1].Head;
			}else if(_MK_Current_Pro_->TaskDelayTicks != 0){
				_MK_Next_Pro_ = &idletask;
			}else{
				return;
			}
		}else if(_MK_Current_Pro_ == _OSReadyList[1].Head){
			if(_OSReadyList[0].Head->TaskDelayTicks == 0){
				_MK_Next_Pro_ = _OSReadyList[0].Head;
			}else if(_MK_Current_Pro_->TaskDelayTicks != 0){
				_MK_Next_Pro_ = &idletask;
			}else{
				return;
			}
		}
	}
	
	_TriggerPendSV_();

}

