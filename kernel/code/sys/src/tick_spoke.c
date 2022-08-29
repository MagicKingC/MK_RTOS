#include <tick_spoke.h>
#include <priority.h>
#include <system.h>

mk_uint32 MK_TickCtr;
MK_TICK_SPOKE MK_TickSpokeList[MK_TickWheelSize];

//��ʼ��ʱ���б�
void InitTickSpokeList(){
	mk_uint32 index = 0;
	for(index = 0; index < MK_TickWheelSize;index++){
		MK_TickSpokeList[index] = (MK_TICK_SPOKE){ MK_NULL, 0, 0 };
	}
}

//���߳̿����ʱ���б� ��С��������
mk_code InsertNodeToTickSpokeList(mk_TaskTcb *TaskTCB, mk_uint32 _time){
	mk_uint32 index = 0;
	mk_TaskTcb *tmpNode;
	mk_TaskTcb *endNode;
	
	TaskTCB->WaitTick = _time;
	TaskTCB->TickCount = MK_TickCtr + _time;
	
	index = TaskTCB->TickCount % MK_TickWheelSize;
	
	tmpNode = MK_TickSpokeList[index].Next;
	
	if(tmpNode == MK_NULL){//�����׸��ڵ�
		MK_TickSpokeList[index].Next = TaskTCB;
		TaskTCB->TickNext = MK_NULL;
	}else{
		while(tmpNode){
			if(tmpNode->WaitTick > TaskTCB->WaitTick){
				//�жϲ����λ��
				if(tmpNode->TickSpoke->TickSpokeNodeNum == 1){//��λ
					tmpNode->TickSpoke->Next = TaskTCB;
				}
				
				TaskTCB->TickPrev = tmpNode->TickPrev;
				tmpNode->TickPrev->TickNext = TaskTCB;
				
				TaskTCB->TickNext = tmpNode;
				tmpNode->TickPrev = TaskTCB;
				
				goto _NEXT;
			}
			endNode = tmpNode;
			tmpNode = tmpNode->TickNext;
		}
		//�����߳̿�Ϊ����һ��
		endNode->TickNext = TaskTCB;
		TaskTCB->TickPrev = endNode;
		TaskTCB->TickNext = MK_NULL;
		
	}
	_NEXT:
		TaskTCB->TickSpoke = &MK_TickSpokeList[index];
		MK_TickSpokeList[index].TickSpokeNodeNum++;
	//MK_TickSpokeList[index].TickSpokeNodeMax++;
	
	return MK_SUCCESS;
}

//���߳̿��ʱ���б��Ƴ�
void RemoveFromTickSpokeList(mk_TaskTcb *TaskTCB){	
	if(TaskTCB == TaskTCB->TickSpoke->Next){//�׽ڵ�
		TaskTCB->TickSpoke->Next = TaskTCB->TickNext;
		if(TaskTCB->TickNext){
			TaskTCB->TickNext->TickPrev = MK_NULL;
		}
	}else{
		TaskTCB->TickPrev->TickNext = TaskTCB->TickNext;
		if(TaskTCB->TickNext){
			TaskTCB->TickNext->TickPrev = TaskTCB->TickPrev;
		}
	}
	TaskTCB->TickPrev = MK_NULL;
	TaskTCB->TickNext = MK_NULL;
	TaskTCB->TickSpoke->TickSpokeNodeNum--;
}

//����ʱ���б�
static void __MK_InsertReady(mk_TaskTcb *TaskTCB){
	RemoveFromTickSpokeList(TaskTCB);
	InsertNodeToReadyListTail(TaskTCB);//��������б�
}

void UpdateToTickSpokeList(void){
	
	mk_TaskTcb *tmpNode;
	mk_TaskTcb *tmpNode2;
	mk_uint32 index = 0;
	

	MK_TickCtr++;
	
	index = MK_TickCtr % MK_TickWheelSize;
	tmpNode = MK_TickSpokeList[index].Next;
	tmpNode2 = tmpNode;
	while(tmpNode2){
		tmpNode2 = tmpNode2->TickNext;
		tmpNode->WaitTick = tmpNode->TickCount - MK_TickCtr;
		if(tmpNode->WaitTick == 0){
			__MK_InsertReady(tmpNode);
		}
		tmpNode = tmpNode2;
	}
}
	
