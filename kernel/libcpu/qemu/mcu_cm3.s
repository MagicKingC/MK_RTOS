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
.global get_msp
.global get_control_reg

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
    CPSID   I
    /*判断psp是否为0，默认系统初始化时会将psp的值赋值为0*/
    MRS	  R0,PSP				/*取出当前栈指针的值*/
    CBZ	  R0,PRO_SWITCH			/*跳到下文切换*/

    /*保存上文*/
    STMDB R0!, {R4-R11}
    /*保存栈指针，到当前栈，用于下次任务切换使用*/
    LDR	  R1,=_MK_Current_Pro_	/*将指向当前的进程控制块的地址加载到R1*/
    LDR	  R1,[R1]				/*将当前的进程控制块地址指向的值加载到R1*/
    STR	  R0,[R1]				/*将当前的进程控制块地址指向的值当作地址，然后往这个以R1的值为地址填入R0的值*/
	
PRO_SWITCH:
    /*下文切换*/
	LDR	  R0,=_MK_Current_Pro_ /* 将当前运行的任务指针的地址值赋值给r0 */
    LDR   R1,=_MK_Highest_Pro_ /* 将下一个运行的任务指针的地址值赋值给r1 */
	LDR   R2,[R1]			   /* 将下一个运行的任务指针赋值给r2*/
	STR	  R2,[R0]			   /* 将下一个运行的任务指针赋值给当前运行的任务指针*/
	
	LDR	  R0,[R2]				/* 将任务栈指针赋值给R0*/
	LDMIA R0!,{R4-R11}			/*将存储器的数据以R3开始地址的往后地址写回到R4到R11寄存器中*/
	
	MSR	  PSP,R0
	ORR	  LR,LR,#0x04			/*是退出中断之后使用PSP堆栈*/
    CPSIE   I
	BX    LR

/*开中断*/
_CPU_InterruptEnable_:
	CPSIE I
	blx    LR
	
/*关中断*/
_CPU_InterruptDisable_:
	CPSID   I
	blx      LR
		
/* 获取保存标志位 */
_MK_SR_Save_:
	MRS		R0,PRIMASK
	blx      LR

/* 设置保存的标志位 */
_MK_SR_Restore_:
	MSR 	PRIMASK,R0
	blx      LR

/* 获取psp堆栈寄存器的值 */
_MK_GET_PSP:
    mrs r0, PSP
    blx lr

/* 设置psp堆栈寄存器的值 */
_MK_SET_PSP:
    msr     PSP, r0
    blx     lr

get_msp:
    mrs r0, MSP
    blx lr

get_control_reg:
    mrs r0, CONTROL
    blx lr
