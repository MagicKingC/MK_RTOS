#ifndef MKIPC_H
#define MKIPC_H

#include <mkinc.h>

#if MK_USE_IPC

typedef struct _MK_OBJECT_ {
    char name[MK_IPC_NAME_LEN];
    mk_list_t list;
} mk_object_t;


int mk_ipc_init(mk_object_t* _obj, const char* _name);

#endif

#endif
