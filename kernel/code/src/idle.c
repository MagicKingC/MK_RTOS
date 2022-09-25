#include <idle.h>
#include <mkrtos.h>

static mk_TaskStack idletaskEnv[32];
mk_TaskTcb idletask;
//空闲任务
void mk_idle_task(void *param){
	while(1){
		
	}
}

void _MK_Idle_Init_(void){
	mk_TaskInit("idle",&idletask,mk_idle_task,MK_NULL,&idletaskEnv[32],MK_PRIORITY_MAX-1,1);
}

