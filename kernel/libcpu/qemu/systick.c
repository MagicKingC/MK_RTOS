#include <cm3.h>
#include <mkrtos.h>

volatile unsigned int * const UART0DR = (unsigned int *)0x4000C000;

__weak char mk_putc(mk_uint8_t *ch)
{
    *UART0DR = *ch;
    return *ch;
}

extern mk_uint32_t _bss;
extern mk_uint32_t _ebss;

/**
 * @brief 清除bss短
 */
static inline void ClearBSS(void)
{
    mk_uint8_t *start = (mk_uint8_t *)_bss;
    while ((mk_uint32_t)start < _ebss)
    {
        *start = 0;
        start++;
    }
}

/**
 * @brief 初始化系统定时器
 * @param ms 
 * @return __weak 
 */
__weak void mk_SystickInit(mk_uint32_t ms)
{
    ClearBSS();
    mk_systick_t *SysTick = (mk_systick_t *)SYSTICK_BASE;
    mk_uint8_t *SysPriorityPionter = (mk_uint8_t *)SYSTICK_PRIO_REG;
    //配置优先级
    *SysPriorityPionter = 0xF0;
    //时间节拍
    SysTick->load = (ms * SystemCoreClock/1000) - 1;
    //当前值
    SysTick->val = 0;
    //选择时钟源（内部时钟）,使能中断，异常使能
    SysTick->ctrl = 0x07;
}

__weak void SysTick_Handler(void)
{
    mk_uint32_t c_res = mk_critical_enter();
    UpdateToTickSpokeList();
#if USE_TIME_SLICE
    _MK_TaskTimeSliceSched(&_MK_ReadyList[_MK_Highest_Prio_Index]);
#endif
    mk_critical_exit(c_res);
    _MK_TaskSwitch_();    
}


/**
 * @brief 首次运行
 */
__weak void _MK_RTOS_RUN_() {
    mk_printk("%s\n", __func__);
    _MK_SET_PSP(0);
    MEM8(NVIC_SYSPRI14) = NVIC_PENDSV_PRI;
    _CPU_InterruptEnable_();
    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}

/**
 * @brief 触发pendSV中断
 */
__weak void _TriggerPendSV_(void) {
    MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}
