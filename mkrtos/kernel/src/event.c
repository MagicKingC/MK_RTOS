#include <mkevent.h>
#include <mkdebug.h>
#include <mklist.h>
#include <mkstr.h>
#include <mktask.h>

mk_code_t mk_event_create(mk_event_t* _event,const char* _name){
    mk_ipc_init(&_event->obj);
    return MK_SUCCESS;
}