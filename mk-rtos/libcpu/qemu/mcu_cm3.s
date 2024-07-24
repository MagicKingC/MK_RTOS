.text
.code 16
.syntax unified

/* import */
.global __MK_Main
.global _p_stack_top

/*import*/
.global _MK_Current_Pro_
.global _MK_Highest_Pro_

/* export */
.global Reset_Handler
.global PendSV_Handler

.global _MK_GET_PSP
.global _MK_SET_PSP
.global _MK_GET_MSP
.global _MK_CONTROL_REG

.global _CPU_InterruptEnable_
.global _CPU_InterruptDisable_
.global _MK_SR_Save_
.global _MK_SR_Restore_

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
    ldr r0, =__MK_Main
    blx r0
    b .

/* pendsv中断处理 */
PendSV_Handler:
    cpsid   I
    /*判断psp是否为0，默认系统初始化时会将psp的值赋值为0*/
    mrs	  r0,psp				/*取出当前栈指针的值*/
    cbz	  r0,PRO_SWITCH			/*跳到下文切换*/

    /*保存上文*/
    STMDB r0!, {R4-R11}
    /*保存栈指针，到当前栈，用于下次任务切换使用*/
    ldr	  R1,=_MK_Current_Pro_	/*将指向当前的进程控制块的地址加载到R1*/
    ldr	  R1,[R1]				/*将当前的进程控制块地址指向的值加载到R1*/
    STR	  r0,[R1]				/*将当前的进程控制块地址指向的值当作地址，然后往这个以R1的值为地址填入r0的值*/
	
PRO_SWITCH:
    /*下文切换*/
	ldr	  r0,=_MK_Current_Pro_ /* 将当前运行的任务指针的地址值赋值给r0 */
    ldr   R1,=_MK_Highest_Pro_ /* 将下一个运行的任务指针的地址值赋值给r1 */
	ldr   R2,[R1]			   /* 将下一个运行的任务指针赋值给r2*/
	STR	  R2,[r0]			   /* 将下一个运行的任务指针赋值给当前运行的任务指针*/
	
	ldr	  r0,[R2]				/* 将任务栈指针赋值给r0*/
	ldmia r0!,{R4-R11}			/*将存储器的数据以R3开始地址的往后地址写回到R4到R11寄存器中*/
	
	msr	  psp,r0
	orr	  lr,lr,#0x04			/*是退出中断之后使用psp堆栈*/
    cpsie   I
	bx    lr

/*开中断*/
_CPU_InterruptEnable_:
	cpsie I
	blx    lr
	
/*关中断*/
_CPU_InterruptDisable_:
	cpsid   I
	blx      lr
		
/* 获取保存标志位 */
_MK_SR_Save_:
	mrs		r0,PRIMASK
	blx      lr

/* 设置保存的标志位 */
_MK_SR_Restore_:
	msr PRIMASK,r0
	blx lr

/* 获取psp堆栈寄存器的值 */
_MK_GET_PSP:
    mrs r0, psp
    blx lr

/* 设置psp堆栈寄存器的值 */
_MK_SET_PSP:
    msr psp, r0
    blx lr

/* 获取msp堆栈寄存器的值 */
_MK_GET_MSP:
    mrs r0, msp
    blx lr

/* 设置msp堆栈寄存器的值 */
_MK_SET_MSP:
    msr msp, r0
    blx lr

_MK_CONTROL_REG:
    mrs r0, CONTROL
    blx lr
