#ifndef SYSTEM_H
#define SYSTEM_H

#include <mktype.h>

MK_RTOS_EXT void _MK_RTOS_RUN_(void);
MK_RTOS_EXT void _TriggerPendSV_(void);
MK_RTOS_EXT	void _CPU_InterruptEnable_(void);
MK_RTOS_EXT	void _CPU_InterruptDisable_(void);

MK_RTOS_EXT mk_uint32 _MK_SR_Save_(void);
MK_RTOS_EXT void _MK_SR_Restore_(mk_uint32 reg_value);


//¡ŸΩÁ∂Œ
#define mk_critical_enter() (\
	{mk_uint32 reg_val;\
	reg_val = _MK_SR_Save_();\
	reg_val;\
	})
	
#define mk_critical_exit(reg_val) do{\
	_MK_SR_Restore_(reg_val);\
	}\
	while(0);

MK_RTOS_EXT void __MK_Main(void);
MK_RTOS_EXT int main(void);

#endif
