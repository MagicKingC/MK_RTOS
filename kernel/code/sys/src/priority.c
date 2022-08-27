#include <priority.h>

//优先级表
static mk_task_tcb _MK_Prio_Table_[MK_PRIORITY_MAX];
//延迟阻塞队列
#define _MK_DELAY_LIST_MAX_ 256
static mk_task_tcb * _MK_Delay_List[_MK_DELAY_LIST_MAX_];
static mk_uint16 _mkDelayIndex = 0;

void _MK_PrioInit_(void){
	mk_uint32 index = 0;
	for(index=0;index < MK_PRIORITY_MAX; index++){
		_MK_Prio_Table_[index].Head = MK_NULL;
		_MK_Prio_Table_[index].Next = MK_NULL;
	}
}

void _MK_DelayInit(void){
	mk_uint16 index = 0;
	for(index=0; index < _MK_DELAY_LIST_MAX_;index++)
	{
		_MK_Delay_List[index]=MK_NULL;
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
			}
			return node;
		}
	}
	return node;
}


/*
将线程模块插入延迟队列
*/
mk_code _MK_InsertToDelayList(mk_task_tcb *TaskTCB){
	mk_uint16 index = 0;
	for(index=0;index<_MK_DELAY_LIST_MAX_;index++){
		if(_MK_Delay_List[index]==MK_NULL){
				_MK_Delay_List[index] = TaskTCB;
				return MK_SUCCESS;
		}
	}
	return MK_FAIL;
}

/*
线程的延迟数减一
*/
void _MK_SUBDelayList(){
	mk_uint16 index = 0;
	for(index = 0;index<_MK_DELAY_LIST_MAX_;index++){
		if(_MK_Delay_List[index]){
			if(_MK_Delay_List[index]->TaskDelayTicks > 0){
				_MK_Delay_List[index]->TaskDelayTicks--;
			}else{
				_MK_InsertTCBToPrioTable(_MK_Delay_List[index]);
				_MK_Delay_List[index] = MK_NULL;
			}
		}
	}
}