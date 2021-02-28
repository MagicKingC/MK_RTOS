#include "system.h"

void mk_taskinit(mk_stack * task,void (*entry)(void *),void *param,mk_taskstack *stack){
	
	task->stack = stack;
	
	*(--stack) = (mk_uint32)(1<<24);//必须置为1，否则进入arm模式，运行异常
	*(--stack) = (mk_uint32)(entry);
	//硬件自动保存
	*(--stack) = (mk_uint32)(0x14);
	*(--stack) = (mk_uint32)(0x12);
	*(--stack) = (mk_uint32)(0x3);
	*(--stack) = (mk_uint32)(0x2);
	*(--stack) = (mk_uint32)(0x1);
	*(--stack) = (mk_uint32)(param);
	//手动保存
	*(--stack) = (mk_uint32)(0x11);
	*(--stack) = (mk_uint32)(0x10);
	*(--stack) = (mk_uint32)(0x9);
	*(--stack) = (mk_uint32)(0x8);
	*(--stack) = (mk_uint32)(0x7);
	*(--stack) = (mk_uint32)(0x6);
	*(--stack) = (mk_uint32)(0x5);
	*(--stack) = (mk_uint32)(0x4);
}

