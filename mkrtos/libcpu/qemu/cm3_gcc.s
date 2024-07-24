.text
.code 16
.syntax unified

/* import */
.global _mk_rtos_sys_main
.global _p_stack_top

/*import*/
.global g_current_task_sp
.global g_next_task_sp

/* 中断处理函数 */
.global Reset_Handler
.global PendSV_Handler
.global SVC_Handler


/*汇编函数 */
.global mk_call_svc
.global mk_disable_interrupt
.global mk_enable_interrupt
.global mk_enter_critical
.global mk_exit_critical
.global mk_trigger_pendsv

.equ syspri14,          0xE000ED22
.equ pendsv_pri,        0xFF
.equ nvic_int_ctrl,     0xE000ED04
.equ set_pendsv,        0x10000000

.type Reset_Handler, %function
Reset_Handler:
    /*Set the stack as process stack*/
    mov r0, #33
    mrs r0, CONTROL
    mov r1, #2
    orr r0, r1
    msr CONTROL, r0

    ldr r0, =_p_stack_top
    mov sp, r0
    /* 跳转的main函数 */
    ldr r0, =_mk_rtos_sys_main
    blx r0
    b .

.type SVC_Handler, %function
SVC_Handler:
    cpsid   i

	/*设置pendsv优先级*/
	ldr		r0,=syspri14
	ldr		r1,=pendsv_pri
	strb	r1,[r0]

    /* 保存下一个运行指针 */
    ldr     r0, =g_current_task_sp /* g_current_task_sp 值赋值 给r0*/
    ldr     r0, [r0]
    ldr     r0, [r0]
    ldmfd   r0!, {r4-r11}

    msr     psp, r0
    orr     lr, lr, #0x04
    cpsie   i
    bx      lr

/*
1、先比较psp堆栈指针判断是否为0，是则表示第一次运行，直接跳到下一个上下文环境
2、否则保存当前上下文环境，然后加载下一个上下文环境
*/
.type PendSV_Handler, %function
PendSV_Handler:
    cpsid   i

    /* 保存上文 */
    mrs     r0, psp
    stmfd   r0!, {r4-r11}
    ldr     r1, =g_current_task_sp
    ldr     r1, [r1]
    str     r0, [r1]

    /* 加载下文 */
    ldr     r0, =g_current_task_sp /* g_current_task_sp 值赋值 给r0*/
    ldr     r1, =g_next_task_sp
    ldr     r2, [r1] /* 取出以r1的值作为地址所指向的值 */
    str     r2, [r0]

    ldr     r0, [r2]
    ldmfd   r0!, {r4-r11}

    msr     psp, r0
    orr     lr, lr, #0x04
    cpsie   i
    bx      lr

/* 用于触发svc中断 */
.type mk_call_svc, %function	
mk_call_svc:
    svc 0
    blx lr

/* 关中断 */
.type mk_disable_interrupt, %function
mk_disable_interrupt:
    cpsid I
    bx lr

/* 开中断 */
.type mk_enable_interrupt, %function
mk_enable_interrupt:
    cpsie i
    bx lr
		
/* 进入临界区 */
.type mk_enter_critical, %function
mk_enter_critical:
    mrs r0,primask
    cpsid i
    bx lr

/* 退出临界区 */
.type mk_exit_critical, %function
mk_exit_critical:
    msr primask,r0
    cpsie i
    bx lr

/* 获取psp堆栈寄存器的值 */
mk_get_psp:
    mrs r0, psp
    blx lr

/* 设置psp堆栈寄存器的值 */
mk_set_psp:
    msr psp, r0
    blx lr

/* 获取msp堆栈寄存器的值 */
mk_get_msp:
    mrs r0, msp
    blx lr

/* 设置msp堆栈寄存器的值 */
mk_set_msp:
    msr msp, r0
    blx lr

/* 触发pendsv */
.type mk_trigger_pendsv, %function
mk_trigger_pendsv:
    ldr r0,=nvic_int_ctrl
    ldr r1,=set_pendsv
    str r1,[r0]
    bx lr