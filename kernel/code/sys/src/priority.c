#include <priority.h>
#include <system.h>

/*****************************�������**********************************/
//��ʼ������
#define _LIST_HEAD_INIT_(list) { .Prev=MK_NULL, .Next=MK_NULL,.TaskNum=0};
#define __MK_InitList(list,node_type) list = (node_type)_LIST_HEAD_INIT_(list)
//�ж������Ƿ�Ϊ��
#define __MK_ListIsEmpty(list) ((((list->Prev == MK_NULL) && (list->Next == MK_NULL))) ? MK_TRUE:MK_FALSE)
//�������ݵ�����
//β�巨
//inline void __MK_InsertToListTail(MK_READY_LIST_NODE *list,MK_READY_LIST_NODE *node){
//	
////	list->Prev->Next = node;
////	node->Next = list;
////	
////	node->Prev = list->Prev;
////	list->Prev = node;
//}
//ɾ������ڵ�
//inline void __MK_DeletFromList(_MK_LIST_NODE *node){
//	node->Next->Prev = node->Prev;
//	node->Prev->Next = node->Next;
//	
//	node->Prev = MK_NULL;
//	node->Next = MK_NULL;
//}
////�����б��нڵ�
//inline mk_bool __MK_FindFromList(_MK_LIST_NODE *list,_MK_LIST_NODE *node){
//	_MK_LIST_NODE *tmpNode;
//	tmpNode = list->Next;
//	while(tmpNode != list){
//		if(tmpNode == node){
//			return MK_TRUE;
//		}
//		tmpNode = tmpNode->Next;
//	}
//	return MK_FALSE;
//}

////ȡ�������һ���ڵ�
//inline _MK_LIST_NODE * GetFirstNodeFromList(_MK_LIST_NODE *list){
//	_MK_LIST_NODE *node;
//	node = list->Next;
//	__MK_DeletFromList(node);
//	return node;
//}
/****************************************************************************/

/**********************************�����б�********************************/
//��ʼ�������б�
void InitReadyList(void){
	mk_uint32 index;
	for(index=0; index < MK_READYLIST_MAX;index++){
		__MK_InitList(_MK_ReadyList[index],MK_READY_LIST_NODE);
	}
}

//���߳̿��������б�(ͷ�巨)
void InsertNodeToReadyListHead(mk_TaskTcb *node){
	SetBitToPrioTable(node->TaskPrio);
	if(_MK_ReadyList[node->TaskPrio].TaskNum == 0){
		_MK_ReadyList->Prev = node;
		_MK_ReadyList->Next = node;
		
		node->Prev = MK_NULL;
		node->Next = MK_NULL;
	}
	else{
		node->Next = _MK_ReadyList->Next->Next;
		node->Prev = _MK_ReadyList->Next;
		_MK_ReadyList->Next->Next = node;
		_MK_ReadyList->Next = node;
	}
	_MK_ReadyList[node->TaskPrio].TaskNum++;
}


//���߳̿��������б�(β�巨)
void InsertNodeToReadyListTail(mk_TaskTcb *node){
	SetBitToPrioTable(node->TaskPrio);
	if(_MK_ReadyList[node->TaskPrio].TaskNum == 0){
		_MK_ReadyList[node->TaskPrio].Prev = node;
		_MK_ReadyList[node->TaskPrio].Next = node;
		
		node->Prev = MK_NULL;
		node->Next = MK_NULL;
	}
	else{
		node->Next = _MK_ReadyList[node->TaskPrio].Next->Next;
		node->Prev = _MK_ReadyList[node->TaskPrio].Next;
		_MK_ReadyList[node->TaskPrio].Next->Next = node;
		_MK_ReadyList[node->TaskPrio].Next = node;
	}
	_MK_ReadyList[node->TaskPrio].TaskNum++;
}

//�������б��ͷ�ƶ���β��
void MoveHeadToTailInReadList(MK_READY_LIST_NODE *list){
	if(list->TaskNum <= 1){
		return;
	}
	mk_TaskTcb *tmpNode = list->Prev;
	list->Prev = list->Prev->Next;
	tmpNode->Prev = list->Next;
	tmpNode->Next = list->Next->Next;
	list->Next->Next = tmpNode;
	list->Next = tmpNode;
}

//���߳̿�Ӿ����б���ɾ��
void RemoveNodeFromReadyList(mk_TaskTcb *node){
	if(_MK_ReadyList[node->TaskPrio].TaskNum == 1){
		_MK_ReadyList[node->TaskPrio].Prev = MK_NULL;
		_MK_ReadyList[node->TaskPrio].Next = MK_NULL;
		ClearBitToPrioTable(node->TaskPrio);//�����ȼ�����ɾ�����߳�
		
	}else if(_MK_ReadyList[node->TaskPrio].TaskNum > 1){
		if(node->Prev == MK_NULL){//�����е��׽ڵ�
			_MK_ReadyList[node->TaskPrio].Prev = node->Next;
		}else if(node->Next == MK_NULL){//�����е�β�ڵ�
			_MK_ReadyList[node->TaskPrio].Next = node->Prev;
		}else{
			node->Prev->Next = node->Next;
			node->Next->Prev = node->Prev;
		}
	}else{
		return;
	}
	_MK_ReadyList[node->TaskPrio].TaskNum--;
}

/************************************************************/

/***********************���ȼ���*****************************/
//��ʼ�����ȼ���
void InitPrioTable(void){
	mk_uint32 index;
	for(index = 0;index < MK_PRIORITY_TABLE_SIZE;index++){
		_MK_PrioTable[index] = 0;
	}
}

//���ҵ�һ����Ϊ0������ (����CLZָ������������)
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


//��ȡ������ȼ�
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

//�������ȼ�
mk_code SetBitToPrioTable(mk_uint32 TaskPrio){
	mk_uint32 index = TaskPrio/MK_PRIORITY_MAX;
	mk_uint32 bit = TaskPrio%32;
	_MK_PrioTable[index] |= 1<< bit; 
	return MK_SUCCESS;
}

//������ȼ�
mk_code ClearBitToPrioTable(mk_uint32 TaskPrio){
	mk_uint32 index = TaskPrio/MK_PRIORITY_MAX;
	mk_uint32 bit = TaskPrio%32;
	_MK_PrioTable[index] &= ~(1<< bit); 
	return MK_SUCCESS;
}



/**********************************************************************************/

//��ʼ���ӳ��б�
void InitDelayList(void){
//	mk_uint32 index;
//	for(index=0; index < MK_DELAYLIST_MAX;index++){
//		__MK_InitList(_MK_DelayList[index],_MK_LIST_NODE);
//	}
}

//���߳̿�����ӳ��б�
void InsertNodeToDelayList(_MK_LIST_NODE *node){
//	if(!__MK_FindFromList(_MK_DelayList,node)){
//		__MK_DeletFromList(node);//�Ӿ����б���ɾ��
//		__MK_InsertToListTail(&_MK_DelayList[node->TaskPrio],node);
//	}
}
mk_bool FindFromDelayList(_MK_LIST_NODE *node)
{
//	return __MK_FindFromList(_MK_DelayList,node);
	return MK_FALSE;
}
//ѭ����һ
void SubDelayList(void){
//	_MK_LIST_NODE *tmpNode;
//	tmpNode = _MK_DelayList->Next;
//	while(tmpNode != _MK_DelayList){
//		if(tmpNode->TaskDelayTicks > 0){
//			tmpNode->TaskDelayTicks--;
//		}else{
//			__MK_DeletFromList(tmpNode);
//			InsertNodeToReadyList(tmpNode);
//		}
//		tmpNode = tmpNode->Next;
//	}
}
