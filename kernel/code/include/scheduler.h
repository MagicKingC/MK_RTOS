#ifndef SWITCH_H
#define SWITCH_H

#include <mktype.h>
#include <mktask.h>

MK_RTOS_EXT void _MK_TaskSwitch_(void);
MK_RTOS_EXT void _MK_TaskTimeSliceSched(MK_READY_LIST_NODE *list);

#endif
