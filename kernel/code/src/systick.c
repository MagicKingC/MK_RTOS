#include <systick.h>
#include <mkrtos.h>

mk_uint32 MK_TickCtr;

/*
  延时
*/

void mk_delay_ms(mk_uint32 ms){
	
	mk_uint32 c_res = mk_critical_enter();
	InsertNodeToTickSpokeList(_MK_Current_Pro_,ms);
	RemoveNodeFromReadyList(_MK_Current_Pro_);
	mk_critical_exit(c_res);
	//放弃运行权
	_MK_TaskSwitch_();
}


/* 
    时间戳 
*/
#if USE_TIME_STAMP

#define	DWT_DEMCR_REG	(*(volatile mk_uint32 *)0XE000EDFC)
#define DWT_CTRL_REG	(*(volatile mk_uint32 *)0XE0001000)
#define DWT_CYCCNT_REG	(*(volatile mk_uint32 *)0XE0001004)
	
#define DWT_ENABLE		(1<<24)
#define DWT_CYCCNTENA	(1<<0)

static mk_uint32 BSP_CPU_ClkFrep(void){
	mk_uint32 system_clock;
	
	system_clock = SystemCoreClock;
	
	return system_clock;
}

void mk_time_stamp_init(void){
	mk_uint32 fclk_frep;
	fclk_frep = BSP_CPU_ClkFrep();
	//初始化DWT
	DWT_DEMCR_REG |= (mk_uint32)DWT_ENABLE;
	DWT_CYCCNT_REG = (mk_uint32)0u;
	DWT_CTRL_REG  &= (~((mk_uint32)DWT_CYCCNTENA));
}

mk_uint32 mk_get_time_stamp(void){
	mk_uint32 time = 0;
	
	time = (mk_uint32)DWT_CYCCNT_REG;
	
	return time;
}

#endif

/*
	时间片
*/
//初始化时基列表
void InitTickSpokeList(){
	mk_uint32 index = 0;
	for(index = 0; index < MK_TickWheelSize;index++){
		MK_TickSpokeList[index] = (MK_TICK_SPOKE){ MK_NULL, 0, 0 };
	}
}

//将线程块插入时基列表 从小到大排序
mk_code_t InsertNodeToTickSpokeList(mk_TaskTcb *TaskTCB, mk_uint32 _time){
	mk_uint32 index = 0;
	mk_TaskTcb *tmpNode;
	mk_TaskTcb *endNode;
	
	TaskTCB->TaskStatus |= MK_TASK_STATE_BIT_DLY;
	TaskTCB->WaitTick = _time;
	TaskTCB->TickCount = MK_TickCtr + _time;
	
	index = TaskTCB->TickCount % MK_TickWheelSize;
	
	tmpNode = MK_TickSpokeList[index].Next;
	
	if(tmpNode == MK_NULL){//插入首个节点
		MK_TickSpokeList[index].Next = TaskTCB;
		TaskTCB->TickNext = MK_NULL;
	}else{
		while(tmpNode){
			if(tmpNode->WaitTick > TaskTCB->WaitTick){
				//判断插入的位置
				if(tmpNode->TickSpoke->TickSpokeNodeNum == 1){//首位
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
		//插入线程块为最大的一个
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

//将线程块从时基列表移除
void RemoveFromTickSpokeList(mk_TaskTcb *TaskTCB){	
	if(TaskTCB == TaskTCB->TickSpoke->Next){//首节点
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

//更新时基列表
static void __MK_InsertReady(mk_TaskTcb *TaskTCB){
	RemoveFromTickSpokeList(TaskTCB);
	InsertNodeToReadyListTail(TaskTCB);//插入就绪列表
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
	
