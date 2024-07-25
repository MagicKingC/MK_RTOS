#ifndef MKEVENT_H
#define MKEVENT_H

#include <mkinc.h>

#if MK_USE_EVENT

typedef struct _MK_EVENT_
{
    char name[MK_EVENT_NAME_LEN];
    mk_list_t sem_list;
    mk_bool_t is_use;
}mk_event_t;


#endif

#endif
