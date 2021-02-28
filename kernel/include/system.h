#ifndef SYSTEM_H
#define SYSTEM_H

#include "data_type.h"
#include "mkrtos.h"

MK_RTOS_EXT void mk_taskinit(mk_stack * task,void (*entry)(void *),void *param,mk_taskstack *stack);

MK_RTOS_EXT void _MK_RTOS_RUN_(void);
MK_RTOS_EXT void _TriggerPendSV_(void);

#endif
