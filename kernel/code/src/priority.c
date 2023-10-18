#include <priority.h>
#include <system.h>

/**********************************�����б�********************************/
//��ʼ�������б�
#define _LIST_HEAD_INIT_(list) { .Prev=MK_NULL, .Next=MK_NULL,.TaskNum=0};
#define __MK_InitList(list,node_type) list = (node_type)_LIST_HEAD_INIT_(list)

void InitReadyList(void){
	mk_uint32_t index;
	for(index=0; index < MK_READYLIST_MAX;index++){
		__MK_InitList(_MK_ReadyList[index],MK_READY_LIST_NODE);
	}
}

//���߳̿��������б�(ͷ�巨)
void InsertNodeToReadyListHead(mk_TaskTcb *node){
	node->TaskStatus |= MK_TASK_STATY_RDY;
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


//���߳̿��������б�(β�巨)
void InsertNodeToReadyListTail(mk_TaskTcb *node){
	node->TaskStatus |= MK_TASK_STATY_RDY;
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

//�������б��ͷ�ƶ���β��
void MoveHeadToTailInReadList(MK_READY_LIST_NODE *list){
	if(list->TaskNum <= 1){
		return;
	}
	mk_TaskTcb *tmpNode = list->Prev;
	list->Prev = list->Prev->ReadyNext;
	list->Prev->ReadyPrev = MK_NULL;
	tmpNode->ReadyPrev = list->Next;
	tmpNode->ReadyNext = list->Next->ReadyNext;
	list->Next->ReadyNext = tmpNode;
	list->Next = tmpNode;
}

//���߳̿�Ӿ����б���ɾ��
void RemoveNodeFromReadyList(mk_TaskTcb *node){
	node->TaskStatus |= MK_TASK_STATY_DEL;
	if(_MK_ReadyList[node->TaskPrio].TaskNum == 1){
		_MK_ReadyList[node->TaskPrio].Prev = MK_NULL;
		_MK_ReadyList[node->TaskPrio].Next = MK_NULL;
		ClearBitToPrioTable(node->TaskPrio);//�����ȼ�����ɾ�����߳�
		
	}else if(_MK_ReadyList[node->TaskPrio].TaskNum > 1){
		if(node->ReadyPrev == MK_NULL){//�����е��׽ڵ�
			_MK_ReadyList[node->TaskPrio].Prev = node->ReadyNext;
		}else if(node->ReadyNext == MK_NULL){//�����е�β�ڵ�
			_MK_ReadyList[node->TaskPrio].Next = node->ReadyPrev;
		}else{
			node->ReadyPrev->ReadyNext = node->ReadyNext;
			node->ReadyNext->ReadyPrev = node->ReadyPrev;
		}
	}else{
		return;
	}
	node->ReadyPrev = MK_NULL;
	node->ReadyNext = MK_NULL;
	_MK_ReadyList[node->TaskPrio].TaskNum--;
	
}

/************************************************************/

/***********************���ȼ���*****************************/
//��ʼ�����ȼ���
void InitPrioTable(void){
	mk_uint32_t index;
	for(index = 0;index < MK_PRIORITY_TABLE_SIZE;index++){
		_MK_PrioTable[index] = 0;
	}
}

//���ҵ�һ����Ϊ0������ (����CLZָ������������)
mk_uint8_t _MK_FindFirstBit_(mk_uint32_t _MK_Priority)
{
	mk_uint8_t index=0;
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

mk_uint32_t _MK_FindFirstBit(const mk_uint32_t *_bitMap){
	mk_uint32_t index=0;
	mk_uint32_t i;
	
	for(i=0;i < 2;i++){
		if(_bitMap[i]){
			index = i*32;
			return (_MK_FindFirstBit_(_bitMap[i]) + index);
		}
	}
	
	return (_MK_FindFirstBit_(_bitMap[i]) + index);
}


//��ȡ������ȼ�
mk_uint32_t GetHighestPrioFromPrioTable(void){
	mk_uint32_t res = 0;
	mk_uint32_t index = 0;
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
mk_code_t SetBitToPrioTable(mk_uint32_t TaskPrio){
	mk_uint32_t index = TaskPrio/MK_PRIORITY_MAX;
	mk_uint32_t bit = TaskPrio%32;
	_MK_PrioTable[index] |= 1<< bit; 
	return MK_SUCCESS;
}

//������ȼ�
mk_code_t ClearBitToPrioTable(mk_uint32_t TaskPrio){
	mk_uint32_t index = TaskPrio/MK_PRIORITY_MAX;
	mk_uint32_t bit = TaskPrio%32;
	_MK_PrioTable[index] &= ~(1<< bit); 
	return MK_SUCCESS;
}

/**********************************************************************************/

