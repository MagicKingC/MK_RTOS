#ifndef TICK_SPOKE_H
#define TICK_SPOKE_H

#include <mkrtos_inc.h>

#define MK_TickWheelSize MK_TICK_WHEEL_SIZE

MK_RTOS_EXT MK_TICK_SPOKE MK_TickSpokeList[MK_TickWheelSize];

//初始化时基列表
MK_RTOS_EXT void InitTickSpokeList();
//将线程块插入时基列表
MK_RTOS_EXT mk_code InsertNodeToTickSpokeList(mk_TaskTcb *TaskTCB, mk_uint32 _time);
//将线程块从时基列表移除
MK_RTOS_EXT void RemoveFromTickSpokeList(mk_TaskTcb *TaskTCB);
//更新时基列表
MK_RTOS_EXT void UpdateToTickSpokeList(void);


#endif
