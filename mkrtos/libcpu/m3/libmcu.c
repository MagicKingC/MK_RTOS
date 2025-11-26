#include <libmcu.h>
#include <mkrtos.h>

volatile unsigned int *const UART0DR = (unsigned int *)0x4000C000;

mk_weak char mk_putc(mk_uint8_t *ch) {
    *UART0DR = *ch;
    return *ch;
}

extern mk_uint32_t _bss;
extern mk_uint32_t _ebss;


/**
 * @brief 初始化系统时钟
 */
void mk_init_systick(void) {
#ifdef MK_USING_SYS_DEBUG
    mkprintk("%s\r\n", __func__);
#endif
    mk_systick_t* systick_base = (mk_systick_t*)(SYSTICK_BASE);
    MEM8(0xE000ED23) = 0x0F;
    systick_base->value = 0x000000;                  // 清空计算值
    systick_base->load = (MK_SYSTEM_MCU_CLK/1000);  // 1ms
}

/**
 * @brief 启动时钟
 */
void mk_start_systick(void) {
    mk_systick_t* systick_base = (mk_systick_t*)(SYSTICK_BASE);
    systick_base->ctrl = 0x07;  // 使能systick定时器，使能中断
}


mk_weak void SysTick_Handler(void) {
    mk_tack_scheduler();
}
