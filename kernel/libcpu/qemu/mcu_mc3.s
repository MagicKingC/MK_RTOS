.text
.code 16
.thumb
.syntax unified

/*import*/
.global _MK_Current_Pro_
.global _MK_Highest_Pro_

/*export*/
.global _TriggerPendSV_
.global _MK_RTOS_RUN_
.global PendSV_Handler
.global _CPU_InterruptEnable_
.global _CPU_InterruptDisable_
.global _MK_SR_Save_
.global _MK_SR_Restore_
	
_MK_RTOS_RUN_:
	/*设置pendsv优先级*/
	LDR		R0,=0xE000ED22
	LDR		R1,=0xFF
	STRB	R1,[R0]
	
	MOVS  R0,#0
	MSR	  PSP,R0

	/*触发pendsv中断*/
	LDR	  R0,=0xE000ED04
	LDR	  R1,=0x10000000
	STR   R1,[R0]
	
	CPSIE   I
		
_TriggerPendSV_:
	/*触发pendsv中断*/
	LDR	  R0,=0xE000ED0
	LDR	  R1,=0x10000000
	STR   R1,[R0]
		
PendSV_Handler:
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
	LDR	  R0,=_MK_Current_Pro_
	LDR   R1,=_MK_Highest_Pro_
	LDR   R2,[R1]
	STR	  R2,[R0]
	
	LDR	  R0,[R2]
	LDMIA R0!,{R4-R11}			/*将存储器的数据以R0开始地址的往后地址写回到R4到R11寄存器中*/
	MSR	  PSP,R0
	ORR	  LR,LR,#0x04			/*是退出中断之后使用PSP堆栈*/
	BX    LR

/*开中断*/
_CPU_InterruptEnable_:
	CPSIE   I
	BX      LR
	
/*关中断*/
_CPU_InterruptDisable_:
	CPSID   I
	BX      LR
		
/*带临界保护的中断*/
_MK_SR_Save_:
	MRS		R0,PRIMASK
	CPSIE   I
	BX      LR

_MK_SR_Restore_:
	MSR 	PRIMASK,R0
	BX      LR

	
