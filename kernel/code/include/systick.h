#ifndef TICK_SPOKE_H
#define TICK_SPOKE_H

#include <mktype.h>
#include <mktask.h>

#define MK_TickWheelSize MK_TICK_WHEEL_SIZE

MK_RTOS_EXT MK_TICK_SPOKE MK_TickSpokeList[MK_TickWheelSize];

/*ʱ��Ƭ*/
//��ʼ��ʱ���б�
MK_RTOS_EXT void InitTickSpokeList();
//���߳̿����ʱ���б�
MK_RTOS_EXT mk_code_t InsertNodeToTickSpokeList(mk_TaskTcb *TaskTCB, mk_uint32 _time);
//���߳̿��ʱ���б��Ƴ�
MK_RTOS_EXT void RemoveFromTickSpokeList(mk_TaskTcb *TaskTCB);
//����ʱ���б�
MK_RTOS_EXT void UpdateToTickSpokeList(void);


/* ��ʱ */
MK_RTOS_EXT void mk_delay_ms(mk_uint32 ms);

/* ��ʼ�� */
MK_RTOS_EXT void mk_SystickInit(mk_uint32 ms);

/* ʱ��� */
#if USE_TIME_STAMP
MK_RTOS_EXT void mk_time_stamp_init(void);
MK_RTOS_EXT mk_uint32 mk_get_time_stamp(void);
#endif

#endif
