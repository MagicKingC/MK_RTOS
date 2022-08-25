#ifndef PRIORITY_H
#define PRIORITY_H

#include <mkrtos_config.h>
#include <mkrtos_inc.h> 

extern void _MK_PrioInit_(void);
extern void _MK_InsertTCBToPrioTable(mk_task_tcb *TaskTCB);
extern mk_task_tcb * _MK_GetTCBFromPrioTable();

#endif
