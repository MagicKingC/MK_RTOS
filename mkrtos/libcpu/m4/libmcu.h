#ifndef LIBMCU_H
#define LIBMCU_H

#include <mkdef.h>

#define SYSTICK_BASE 0xE000E010

typedef struct _MK_SYSTICK_BASE {
    __MK_IO32 ctrl;   // 控制寄存器
    __MK_IO32 load;   // 加载寄存器
    __MK_IO32 value;  // 数值寄存器
    __MK_IO32 cal;    // 校准寄存器
} mk_systick_t;

MK_RTOS_EXT void mk_init_systick(void);
MK_RTOS_EXT void mk_start_systick(void);


#endif /*LIBMCU_H*/
