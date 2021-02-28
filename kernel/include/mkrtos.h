#ifndef MKRTOS_H
#define MKRTOS_H

#include "data_type.h"


typedef struct _MK_Task_{
	mk_taskstack *stack;
}mk_stack;

MK_RTOS_EXT mk_stack *_MK_Current_Pro_;
MK_RTOS_EXT mk_stack *_MK_Next_Pro_;

#endif
