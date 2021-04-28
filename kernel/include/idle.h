#ifndef IDLE_H
#define IDLE_H

#include "mkrtos_inc.h"

MK_RTOS_EXT void mk_idle_init(void); 
MK_RTOS_EXT mk_task_tcb idletask;

#endif
