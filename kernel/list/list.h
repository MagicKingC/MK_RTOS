#ifndef LIST_H
#define LIST_H

#include "mkrtos.h"


//子节点
typedef struct _MK_LIST_NODE_ LIST_NODE;
struct _MK_LIST_NODE_{
	mk_uint32 value;
	LIST_NODE *NextNode;		//下一个节点
	LIST_NODE *PreviousNode;	//上一个节点
	void *TCBPoint;
	
};

//头节点
typedef struct _MK_LIST_HREAD_ LIST_HREAD;
struct _MK_LIST_HREAD_{
	mk_uint32 value;
	LIST_HREAD *NextNode;		//下一个节点
	LIST_HREAD *PreviousNode;	//上一个节点
};

MK_RTOS_EXT LIST_NODE * TCP_LIST;

#endif
