#ifndef TIME_STAMP_H
#define TIME_STAMP_H

#include <mkrtos_config.h>
#include <mkrtos_inc.h>

#if USE_TIME_STAMP
MK_RTOS_EXT void mk_time_stamp_init(void);
MK_RTOS_EXT mk_uint32 mk_get_time_stamp(void);
#endif

#endif

