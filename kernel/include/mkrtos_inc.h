#ifndef MKRTOS_INC_H
#define MKRTOS_INC_H

#include "data_type.h"
#include <mkrtos_config.h>

typedef struct _MK_Task_ mk_task_tcb;

struct _MK_Task_{
	mk_TaskStack *Stack;//������ڽṹ���һ��λ�ã����㽫��ַ��ֵ����ջָ��
	mk_uint8  TaskLevel;//�߳����ȼ�
	mk_uint32 TaskDelayTicks;//����ʱʱ��
	mk_task_tcb	*Head;
	mk_task_tcb	*Next;
};

//��������
//�б�ڵ�
//typedef struct os_list_node{
//	mk_task_tcb	*data;
//	struct os_list_node	*Head;
//	struct os_list_node	*Next;
//}_OS_LIST_NODE;
//�б�
MK_RTOS_EXT mk_task_tcb _OSReadyList[MK_PRIORITY_MAX];

MK_RTOS_EXT mk_task_tcb *_MK_Current_Pro_;
MK_RTOS_EXT mk_task_tcb *_MK_Next_Pro_;

#endif
