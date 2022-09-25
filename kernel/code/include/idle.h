#ifndef IDLE_H
#define IDLE_H

#include <mktask.h>

MK_RTOS_EXT void _MK_Idle_Init_(void); 
MK_RTOS_EXT mk_TaskTcb idletask;

#endif
