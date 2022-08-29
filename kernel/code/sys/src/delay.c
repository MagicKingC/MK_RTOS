#include "delay.h"
#include "mkrtos_inc.h"
#include <switch.h>
#include <priority.h>
#include <system.h>
#include <tick_spoke.h>

void mk_delay_ms(mk_uint32 ms){
	
	mk_uint32 c_res = mk_critical_enter();
	InsertNodeToTickSpokeList(_MK_Current_Pro_,ms);
	RemoveNodeFromReadyList(_MK_Current_Pro_);
	mk_critical_exit(c_res);
	//放弃运行权
	_MK_TaskSwitch_();
}
