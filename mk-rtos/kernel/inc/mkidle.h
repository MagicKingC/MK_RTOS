#ifndef IDLE_H
#define IDLE_H

#include <mktask.h>

MK_RTOS_EXT void mk_Idle_Task_Init(void); 
MK_RTOS_EXT mk_TaskTcb idletask;

#endif
