#include <tick_spoke.h>
#include <mkrtos_config.h>

//初始化时基列表
void InitTickSpokeList(){
	mk_uint32 index = 0;
	for(index = 0; index < MK_TickWheelSize;index++){
		MK_TickSpokeList[index] = (MK_TICK_SPOKE){ MK_NULL, 0, 0 };
	}
}

//将线程块插入时基列表
mk_code InsertNodeToTickSpokeList(mk_TaskTcb *TaskTCB, mk_uint32 _time){
	mk_uint32 index = 0;
	mk_TaskTcb *tmpNode;
	mk_TaskTcb *endNode;
	
	TaskTCB->WaitTick = _time;
	TaskTCB->TickCount = MK_TickCtr + _time;
	
	index = TaskTCB->TickCount % MK_TickWheelSize;
	
	if(MK_TickSpokeList[index].TickSpokeNodeNum == 0){
		MK_TickSpokeList[index].Next = TaskTCB;
	}else{
		tmpNode = MK_TickSpokeList[index].Next;
		while(tmpNode){
			
			if(tmpNode->WaitTick > TaskTCB->WaitTick){
				tmpNode->TickPrev->TickNext = TaskTCB;
				TaskTCB->TickPrev = tmpNode->TickPrev;
				
				TaskTCB->TickNext = tmpNode;
				tmpNode->TickPrev = TaskTCB;
				
				goto _NEXT;
			}
			endNode = tmpNode;
			tmpNode = tmpNode->TickNext;
		}
		//最大的一个
		endNode->TickNext = TaskTCB;
		TaskTCB->TickPrev = endNode;
	}
	_NEXT:
		TaskTCB->TickSpoke = &MK_TickSpokeList[index];
	
	return MK_SUCCESS;
}

//将线程块从时基列表移除
void RemoveFromTickSpokeList(mk_TaskTcb *TaskTCB){

}

//更新时基列表
void UpdateToTickSpokeList(void){


}
