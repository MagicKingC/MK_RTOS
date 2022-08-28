#include <idle.h>
#include <system.h>
#include <mkrtos_config.h>
#include <priority.h>

static mk_TaskStack idletaskEnv[128];
mk_TaskTcb idletask;
//空闲任务
void mk_idle_task(void *param){
	while(1){
		
	}
}

void _MK_Idle_Init_(void){

	mk_TaskInit("idle",&idletask,mk_idle_task,&idletaskEnv[128],MK_PRIORITY_MAX-1,(void *)1);
}

