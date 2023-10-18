#include <cm3.h>
#include <mkrtos.h>

volatile unsigned int * const UART0DR = (unsigned int *)0x4000C000;

char mk_putc(mk_uint8_t *ch)
{
    *UART0DR = *ch;
    return *ch;
}

void init_systick()
{
    systick_t *systick_p = (systick_t *)SYSTICK_BASE;
    mk_uint8_t *sys_prio_p = (mk_uint8_t *)SYSTICK_PRIO_REG;
    *sys_prio_p = 0xf0;
    systick_p->load = (HSI_CLK & 0xffffffUL) - 1;
    systick_p->val = 0;
    systick_p->ctrl = 0x7;
}

void systick_handler(void)
{
    mk_printk("systick_handler\n");
}

// void trigger_pend_sv(void)
// {
//     MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI; /*Set PENDSVC loweset priority*/
//     //MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET; /*Trigger PendSV*/
//     *((uint32_t volatile *)0xE000ED04) = 0x10000000;
// }
