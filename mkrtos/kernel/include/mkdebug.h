#ifndef MK_PRINTK_H
#define MK_PRINTK_H

#include <mkdef.h>

MK_RTOS_EXT void mkprintk(const char *fmt, ...);
MK_RTOS_EXT void mk_init_log(void);

#endif