#ifndef TICK_SPOKE_H
#define TICK_SPOKE_H

#include <mkrtos_inc.h>

#define MK_TickWheelSize MK_TICK_WHEEL_SIZE

MK_RTOS_EXT mk_uint32 MK_TickCtr;

typedef struct __MK_TICK_SPOKE__ MK_TICK_SPOKE;

struct __MK_TICK_SPOKE__{
	mk_TaskTcb *Next;
	mk_uint32 TickSpokeNodeNum;
	mk_uint32 TickSpokeNodeMax;
};

MK_RTOS_EXT MK_TICK_SPOKE MK_TickSpokeList[MK_TICK_WHEEL_SIZE];

//��ʼ��ʱ���б�
MK_RTOS_EXT void InitTickSpokeList();
//���߳̿����ʱ���б�
MK_RTOS_EXT mk_code InsertNodeToTickSpokeList(mk_TaskTcb *TaskTCB, mk_uint32 _time);
//���߳̿��ʱ���б��Ƴ�
MK_RTOS_EXT void RemoveFromTickSpokeList(mk_TaskTcb *TaskTCB);
//����ʱ���б�
MK_RTOS_EXT void UpdateToTickSpokeList(void);


#endif
