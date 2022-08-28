#include <priority.h>
#include <system.h>

/**********************************就绪列表********************************/
//初始化就绪列表
#define _LIST_HEAD_INIT_(list) { .Prev=MK_NULL, .Next=MK_NULL,.TaskNum=0};
#define __MK_InitList(list,node_type) list = (node_type)_LIST_HEAD_INIT_(list)

void InitReadyList(void){
	mk_uint32 index;
	for(index=0; index < MK_READYLIST_MAX;index++){
		__MK_InitList(_MK_ReadyList[index],MK_READY_LIST_NODE);
	}
}

//将线程块插入就绪列表(头插法)
void InsertNodeToReadyListHead(mk_TaskTcb *node){
	SetBitToPrioTable(node->TaskPrio);
	if(_MK_ReadyList[node->TaskPrio].TaskNum == 0){
		_MK_ReadyList->Prev = node;
		_MK_ReadyList->Next = node;
		
		node->ReadyPrev = MK_NULL;
		node->ReadyNext = MK_NULL;
	}
	else{
		node->ReadyNext = _MK_ReadyList->Next->ReadyNext;
		node->ReadyPrev = _MK_ReadyList->Next;
		_MK_ReadyList->Next->ReadyNext = node;
		_MK_ReadyList->Next = node;
	}
	_MK_ReadyList[node->TaskPrio].TaskNum++;
}


//将线程块插入就绪列表(尾插法)
void InsertNodeToReadyListTail(mk_TaskTcb *node){
	SetBitToPrioTable(node->TaskPrio);
	if(_MK_ReadyList[node->TaskPrio].TaskNum == 0){
		_MK_ReadyList[node->TaskPrio].Prev = node;
		_MK_ReadyList[node->TaskPrio].Next = node;
		
		node->ReadyPrev = MK_NULL;
		node->ReadyNext = MK_NULL;
	}
	else{
		node->ReadyNext = _MK_ReadyList[node->TaskPrio].Next->ReadyNext;
		node->ReadyPrev = _MK_ReadyList[node->TaskPrio].Next;
		_MK_ReadyList[node->TaskPrio].Next->ReadyNext = node;
		_MK_ReadyList[node->TaskPrio].Next = node;
	}
	_MK_ReadyList[node->TaskPrio].TaskNum++;
}

//将就绪列表的头移动到尾部
void MoveHeadToTailInReadList(MK_READY_LIST_NODE *list){
	if(list->TaskNum <= 1){
		return;
	}
	mk_TaskTcb *tmpNode = list->Prev;
	list->Prev = list->Prev->ReadyNext;
	tmpNode->ReadyPrev = list->Next;
	tmpNode->ReadyNext = list->Next->ReadyNext;
	list->Next->ReadyNext = tmpNode;
	list->Next = tmpNode;
}

//将线程块从就绪列表中删除
void RemoveNodeFromReadyList(mk_TaskTcb *node){
	if(_MK_ReadyList[node->TaskPrio].TaskNum == 1){
		_MK_ReadyList[node->TaskPrio].Prev = MK_NULL;
		_MK_ReadyList[node->TaskPrio].Next = MK_NULL;
		ClearBitToPrioTable(node->TaskPrio);//从优先级表中删除该线程
		
	}else if(_MK_ReadyList[node->TaskPrio].TaskNum > 1){
		if(node->ReadyPrev == MK_NULL){//链表中的首节点
			_MK_ReadyList[node->TaskPrio].Prev = node->ReadyNext;
		}else if(node->ReadyNext == MK_NULL){//链表中的尾节点
			_MK_ReadyList[node->TaskPrio].Next = node->ReadyPrev;
		}else{
			node->ReadyPrev->ReadyNext = node->ReadyNext;
			node->ReadyNext->ReadyPrev = node->ReadyPrev;
		}
	}else{
		return;
	}
	_MK_ReadyList[node->TaskPrio].TaskNum--;
}

/************************************************************/

/***********************优先级表*****************************/
//初始化优先级表
void InitPrioTable(void){
	mk_uint32 index;
	for(index = 0;index < MK_PRIORITY_TABLE_SIZE;index++){
		_MK_PrioTable[index] = 0;
	}
}

//查找第一个不为0的数据 (后用CLZ指令代替这个函数)
mk_uint8 _MK_FindFirstBit_(mk_uint32 _MK_Priority)
{
	mk_uint8 index=0;
	if((_MK_Priority & 0xFFFF) == 0){
		index += 16;
		_MK_Priority >>=16;
	}
	
	if((_MK_Priority & 0xFF) == 0){
		index += 8;
		_MK_Priority >>=8;
	}
	
	if((_MK_Priority & 0xF) == 0){
		index += 4;
		_MK_Priority >>=4;
	}
	
	if((_MK_Priority & 0x3) == 0){
		index += 2;
		_MK_Priority >>=2;
	}
	
	if((_MK_Priority & 0x1) == 0){
		index += 1;
	}
	
	return index;
}

mk_uint32 _MK_FindFirstBit(const mk_uint32 *_bitMap){
	mk_uint32 index=0;
	mk_uint32 i;
	
	for(i=0;i < 2;i++){
		if(_bitMap[i]){
			index = i*32;
			return (_MK_FindFirstBit_(_bitMap[i]) + index);
		}
	}
	
	return (_MK_FindFirstBit_(_bitMap[i]) + index);
}


//获取最高优先级
mk_uint32 GetHighestPrioFromPrioTable(void){
	mk_uint32 res = 0;
	mk_uint32 index = 0;
	for(index = 0;index < MK_PRIORITY_TABLE_SIZE; index++){
		res = _MK_FindFirstBit_(_MK_PrioTable[index]);
		if(res){
			res = res+32*index;
			break;
		}
	}
	return res;
}

//设置优先级
mk_code SetBitToPrioTable(mk_uint32 TaskPrio){
	mk_uint32 index = TaskPrio/MK_PRIORITY_MAX;
	mk_uint32 bit = TaskPrio%32;
	_MK_PrioTable[index] |= 1<< bit; 
	return MK_SUCCESS;
}

//清除优先级
mk_code ClearBitToPrioTable(mk_uint32 TaskPrio){
	mk_uint32 index = TaskPrio/MK_PRIORITY_MAX;
	mk_uint32 bit = TaskPrio%32;
	_MK_PrioTable[index] &= ~(1<< bit); 
	return MK_SUCCESS;
}



/**********************************************************************************/
