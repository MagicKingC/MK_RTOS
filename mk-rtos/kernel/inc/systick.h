#ifndef TICK_SPOKE_H
#define TICK_SPOKE_H

#include <mkdef.h>
#include <mktask.h>

#define MK_TickWheelSize MK_TICK_WHEEL_SIZE

MK_RTOS_EXT MK_TICK_SPOKE MK_TickSpokeList[MK_TickWheelSize];

/*时间片*/
//初始化时基列表
MK_RTOS_EXT void InitTickSpokeList();
//将线程块插入时基列表
MK_RTOS_EXT mk_code_t InsertNodeToTickSpokeList(mk_TaskTcb *TaskTCB, mk_uint32_t _time);
//将线程块从时基列表移除
MK_RTOS_EXT void RemoveFromTickSpokeList(mk_TaskTcb *TaskTCB);
//更新时基列表
MK_RTOS_EXT void UpdateToTickSpokeList(void);


/* 延时 */
MK_RTOS_EXT void mk_delay_ms(mk_uint32_t ms);

/* 初始化 */
MK_RTOS_EXT void mk_SystickInit(mk_uint32_t ms);

/* 时间戳 */
#if USE_TIME_STAMP
MK_RTOS_EXT void mk_time_stamp_init(void);
MK_RTOS_EXT mk_uint32_t mk_get_time_stamp(void);
#endif

#endif
