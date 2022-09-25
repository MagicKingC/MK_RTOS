#ifndef RT_TASK_H
#define RT_TASK_H

#include <mktype.h>
#include <mkrtos_config.h>

//����״̬
#define MK_TASK_STATE_BIT_DLY			0x01u //�ӳ�/��ʱλ
#define MK_TASK_STATE_BIT_PEND			0x02u //�ȴ�λ
#define MK_TASK_STATE_BIT_SUSPENDED		0x04u //����λ

#define MK_TASK_STATY_RDY						0u //����
#define MK_TASK_STATY_DLY						1u //��ʱ/��ʱ
#define MK_TASK_STATY_PEND						2u //�ȴ�
#define MK_TASK_STATY__PEND_TIMEOUT				3u //�ȴ�+��ʱ
#define MK_TASK_STATY_SUSPEND					4u //����
#define MK_TASK_STATY_DLY_SUSPEND				5u //����+��ʱ/��ʱ
#define MK_TASK_STATY_PEND_SUSPENDED			6u //����+�ȴ�
#define MK_TASK_STATY_PEND_TIMEOUT_SUSPENDED	7u //����+�ȴ�+��ʱ
#define MK_TASK_STATY_DEL						255u //ɾ��

typedef struct __MK_TICK_SPOKE__ MK_TICK_SPOKE;
typedef struct _MK_Task_TCB_ mk_TaskTcb;
typedef mk_TaskTcb _MK_LIST_NODE;

struct _MK_Task_TCB_{
	
	volatile mk_TaskStack *TaskStack;//�߳�ջ
	char TaskName[MK_TASK_NAME_LEN];//�߳���
	mk_TaskPrio  TaskPrio;//�߳����ȼ�
	
	//�����б�
	struct _MK_Task_TCB_ *ReadyPrev;
	struct _MK_Task_TCB_ *ReadyNext;
	
	//ʱ���б�
	struct _MK_Task_TCB_ *TickPrev;
	struct _MK_Task_TCB_ *TickNext;
	struct __MK_TICK_SPOKE__ *TickSpoke;
	mk_TaskTick WaitTick;
	mk_TaskTick TickCount;
	
	//ʱ��Ƭ
	mk_TaskTime TaskTimeSlice;	/*ʣ��ʱ��Ƭ*/
	mk_TaskTime TaskMaxTimeSlice;/*��ʱ��Ƭ*/
	
	
	mk_TaskStatus TaskStatus;/*����״̬*/
	
	//�������
	
};

/* ʱ��Ƭ�б� */
struct __MK_TICK_SPOKE__{
	mk_TaskTcb *Next;
	mk_uint32 TickSpokeNodeNum;
	mk_uint32 TickSpokeNodeMax;
};

/* �������ȼ��б� */
typedef struct _MK_READY_LIST_NODE_ MK_READY_LIST_NODE;
struct _MK_READY_LIST_NODE_{
	mk_TaskTcb *Prev;
	mk_TaskTcb *Next;
	mk_uint32 TaskNum;
};


MK_RTOS_EXT mk_uint32 _MK_Highest_Prio_Index;/* ����������ȼ���ѡ�� */

MK_RTOS_EXT mk_TaskTcb *_MK_Current_Pro_;/* ��ǰ����� */
MK_RTOS_EXT mk_TaskTcb *_MK_Highest_Pro_;/* ������ȼ����� */

typedef struct _MK_TASK_{
	mk_TaskTcb task_tcb;		/* ������ƿ� */
	mk_TaskStack *task_stack;	/* �����ջ */
	mk_TaskPrio task_prio;		/* �������ȼ� */
	mk_TaskTime task_time_slice;/* ����ʱ��Ƭ */
	mk_bool 	is_auto_stck;	/* �Ƿ��Զ���������ջ MK_FALSE:���Զ����� MK_TRUE:�Զ����� */
	mk_uint32	task_stck_size; /* �Ƿ�����ջ��С��������õ��Ǿ�̬��ջ�����ֶβ������� */
	char *task_name;			/* �������֣���󳤶���mkrtos_config.h��MK_TASK_NAME_LEN���� */
	void (*task_entry)(void *); /* ������ں��� */
	void * param;				/* ������� */
}mk_task_t;

/*�����ʼ��*/
//�ɰ������ʼ��
MK_RTOS_EXT mk_code_t mk_TaskInit(char * TaskName, mk_TaskTcb * TaskTCB, void (*_entry)(void *), void *_param,\
					mk_TaskStack *_TaskStack, mk_uint32 _TaskPrio ,mk_uint32 _TaskTimeSlice);

MK_RTOS_EXT mk_code_t mk_Task_Init(mk_task_t *_task_struct);

MK_RTOS_EXT void mk_Task_Delete(mk_TaskTcb *_task);

#endif
