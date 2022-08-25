#include <priority.h>

//优先级表
static mk_task_tcb _MK_Prio_Table_[MK_PRIORITY_MAX];

void _MK_PrioInit_(void){
	mk_uint32 index = 0;
	for(index=0;index < MK_PRIORITY_MAX; index++){
		_MK_Prio_Table_[index].Head = MK_NULL;
		_MK_Prio_Table_[index].Next = MK_NULL;
	}
}

//将数据插入优先级表
//采用尾插法
void _MK_InsertTCBToPrioTable(mk_task_tcb *TaskTCB){
	if(_MK_Prio_Table_[TaskTCB->TaskLevel].Next == MK_NULL){
		_MK_Prio_Table_[TaskTCB->TaskLevel].Head = TaskTCB;
		_MK_Prio_Table_[TaskTCB->TaskLevel].Next = TaskTCB;


		TaskTCB->Head = &_MK_Prio_Table_[TaskTCB->TaskLevel];
		TaskTCB->Next = &_MK_Prio_Table_[TaskTCB->TaskLevel];
	}else{
		TaskTCB->Head = _MK_Prio_Table_[TaskTCB->TaskLevel].Next;
		_MK_Prio_Table_[TaskTCB->TaskLevel].Next->Next = TaskTCB;

		TaskTCB->Next = &_MK_Prio_Table_[TaskTCB->TaskLevel];
		_MK_Prio_Table_[TaskTCB->TaskLevel].Next = TaskTCB;
	}
}

/*
从就绪列表中取出一个优先级最高的线程,并且将它从就绪列表中移除
*/
mk_task_tcb * _MK_GetTCBFromPrioTable(){
	mk_uint8 index = 0;
	mk_task_tcb *node = MK_NULL;
	for (index = 0; index < MK_PRIORITY_MAX; index++)
	{
		if (_MK_Prio_Table_[index].Head)
		{
			node = _MK_Prio_Table_[index].Head;
			_MK_Prio_Table_[index].Head = node->Next;
			if (_MK_Prio_Table_[index].Head == &_MK_Prio_Table_[index])
			{
				_MK_Prio_Table_[index].Head = MK_NULL;
				_MK_Prio_Table_[index].Next = MK_NULL;
				continue;
			}
			return node;
		}
		
	}
	return node;
}

