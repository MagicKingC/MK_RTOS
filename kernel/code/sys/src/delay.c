#include "delay.h"
#include "mkrtos_inc.h"
#include "switch.h"
#include <priority.h>

void mk_delay_ms(mk_uint32 ms){
	_MK_Current_Pro_->TaskDelayTicks = ms;
	_MK_InsertToDelayList(_MK_Current_Pro_);
	//放弃运行权
	_mk_task_switch_();
}
