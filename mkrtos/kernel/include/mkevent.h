#ifndef MKEVENT_H
#define MKEVENT_H

#include <mkipc.h>

#if MK_USE_EVENT

typedef struct _MK_EVENT_ {
    mk_object_t obj;
} mk_event_t;

mk_code_t mk_event_create(mk_event_t* _event,const char* _name);

#endif

#endif
