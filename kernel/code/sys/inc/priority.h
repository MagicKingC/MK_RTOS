#ifndef PRIORITY_H
#define PRIORITY_H

#include <mkrtos_config.h>
#include <mkrtos_inc.h> 

#define MK_PRIORITY_TABLE_SIZE MK_PRIORITY_MAX/32
#define MK_READYLIST_MAX MK_PRIORITY_MAX

typedef struct _MK_READY_LIST_NODE_ MK_READY_LIST_NODE;
struct _MK_READY_LIST_NODE_{
	mk_TaskTcb *Prev;
	mk_TaskTcb *Next;
	mk_uint32 TaskNum;
};

//ϵͳ��
MK_RTOS_EXT MK_READY_LIST_NODE _MK_ReadyList[MK_READYLIST_MAX];//�����б�
MK_RTOS_EXT mk_uint32 _MK_PrioTable[MK_PRIORITY_TABLE_SIZE];//���ȼ���

//��ʼ�����ȼ���
MK_RTOS_EXT void InitPrioTable(void);
//��ȡ������ȼ�
MK_RTOS_EXT mk_uint32 GetHighestPrioFromPrioTable(void);
//�������ȼ�
MK_RTOS_EXT mk_code SetBitToPrioTable(mk_uint32 TaskPrio);
//������ȼ�
MK_RTOS_EXT mk_code ClearBitToPrioTable(mk_uint32 TaskPrio);

//��ʼ�������б�
MK_RTOS_EXT void InitReadyList(void);
//���߳̿��������б�(ͷ�巨)
MK_RTOS_EXT void InsertNodeToReadyListHead(mk_TaskTcb *node);
//���߳̿��������б�(β�巨)
MK_RTOS_EXT void InsertNodeToReadyListTail(mk_TaskTcb *node);
//�������б��ͷ�ƶ���β��
MK_RTOS_EXT void MoveHeadToTailInReadList(MK_READY_LIST_NODE *list);
//���߳̿�Ӿ����б���ɾ��
MK_RTOS_EXT void RemoveNodeFromReadyList(mk_TaskTcb *node);



#endif
