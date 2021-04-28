#include "idle.h"
#include "system.h"

static mk_taskstack idletaskEnv[128];
mk_task_tcb idletask;

void mk_idle_task(void *param){

	while(1);
}

void mk_idle_init(void){

	mk_task_init(&idletask,mk_idle_task,(void *)1,&idletaskEnv[128]);
}

