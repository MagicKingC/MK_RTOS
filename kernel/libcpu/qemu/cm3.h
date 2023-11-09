#ifndef CM3_H
#define CM3_H

#include <mktype.h>

#define SCS_BASE            (0xE000E000)         /*System Control Space Base Address */
#define SYSTICK_BASE        (SCS_BASE + 0x0010) /*SysTick Base Address*/
#define SCB_BASE            (SCS_BASE + 0x0D00)
#define SystemCoreClock     12000000UL
#define SYSTICK_PRIO_REG    (0xE000ED23)

#define NVIC_INT_CTRL   0xE000ED04
#define NVIC_PENDSVSET  0x10000000
#define NVIC_SYSPRI14   0xE000ED22
#define NVIC_PENDSV_PRI 0x000000FF

#define MEM32(addr) *(__IO  *)(addr)
#define MEM8(addr)  *(__IO8 *)(addr)

typedef struct systick_tag {
    volatile mk_uint32_t ctrl;
    volatile mk_uint32_t load;
    volatile mk_uint32_t val;
    volatile mk_uint32_t calrb;
} mk_systick_t;

extern mk_uint32_t get_psp(void);
extern mk_uint32_t get_msp(void);
extern void set_psp(mk_uint32_t psp);
extern mk_uint32_t get_control_reg(void);
#endif /*CM3_H*/
