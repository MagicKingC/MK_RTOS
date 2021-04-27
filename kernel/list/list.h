#ifndef LIST_H
#define LIST_H

#include "mkrtos.h"


//�ӽڵ�
typedef struct _MK_LIST_NODE_ LIST_NODE;
struct _MK_LIST_NODE_{
	mk_uint32 value;
	LIST_NODE *NextNode;		//��һ���ڵ�
	LIST_NODE *PreviousNode;	//��һ���ڵ�
	void *TCBPoint;
	
};

//ͷ�ڵ�
typedef struct _MK_LIST_HREAD_ LIST_HREAD;
struct _MK_LIST_HREAD_{
	mk_uint32 value;
	LIST_HREAD *NextNode;		//��һ���ڵ�
	LIST_HREAD *PreviousNode;	//��һ���ڵ�
};

MK_RTOS_EXT LIST_NODE * TCP_LIST;

#endif
