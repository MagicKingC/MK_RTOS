#ifndef PRIORITY_H
#define PRIORITY_H

#include <mkrtos_config.h>
#include <mktask.h> 

#define MK_PRIORITY_TABLE_SIZE MK_PRIORITY_MAX/32
#define MK_READYLIST_MAX MK_PRIORITY_MAX


//系统表
MK_RTOS_EXT MK_READY_LIST_NODE _MK_ReadyList[MK_READYLIST_MAX];//就绪列表
MK_RTOS_EXT mk_uint32_t _MK_PrioTable[MK_PRIORITY_TABLE_SIZE];//优先级表

//初始化优先级表
MK_RTOS_EXT void InitPrioTable(void);
//获取最高优先级
MK_RTOS_EXT mk_uint32_t GetHighestPrioFromPrioTable(void);
//设置优先级
MK_RTOS_EXT mk_code_t SetBitToPrioTable(mk_uint32_t TaskPrio);
//清除优先级
MK_RTOS_EXT mk_code_t ClearBitToPrioTable(mk_uint32_t TaskPrio);

//初始化就绪列表
MK_RTOS_EXT void InitReadyList(void);
//将线程块插入就绪列表(头插法)
MK_RTOS_EXT void InsertNodeToReadyListHead(mk_TaskTcb *node);
//将线程块插入就绪列表(尾插法)
MK_RTOS_EXT void InsertNodeToReadyListTail(mk_TaskTcb *node);
//将就绪列表的头移动到尾部
MK_RTOS_EXT void MoveHeadToTailInReadList(MK_READY_LIST_NODE *list);
//将线程块从就绪列表中删除
MK_RTOS_EXT void RemoveNodeFromReadyList(mk_TaskTcb *node);



#endif
