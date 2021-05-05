	EXPORT _TriggerPendSV_
	EXPORT _MK_RTOS_RUN_
	EXPORT PendSV_Handler
		
	IMPORT _MK_Current_Pro_
	IMPORT _MK_Next_Pro_
		
NVIC_INT_CTRL	EQU		0xE000ED04
NVIC_PENDSVSET	EQU		0x10000000
NVIC_SYSPRI14	EQU		0xE000ED22
NVIC_PENDSV_PRI	EQU			  0xFF
	
	PRESERVE8
	THUMB
	
	AREA CODE,CODE,READONLY

		
_MK_RTOS_RUN_ PROC

	;设置pendsv优先级
	LDR		R0,=NVIC_SYSPRI14
	LDR		R1,=NVIC_PENDSV_PRI
	STRB	R1,[R0]
	
	MOVS  R0,#0
	MSR	  PSP,R0

	;触发pendsv中断
	LDR	  R0,=NVIC_INT_CTRL
	LDR	  R1,=NVIC_PENDSVSET
	STR   R1,[R0]
	
	CPSIE   I
	ENDP
		
_TriggerPendSV_ PROC
	;触发pendsv中断
	LDR	  R0,=NVIC_INT_CTRL
	LDR	  R1,=NVIC_PENDSVSET
	STR   R1,[R0]
	ENDP
		
PendSV_Handler PROC
	;判断psp是否为0，默认系统初始化时会将psp的值赋值为0
	MRS	  R0,PSP				;取出当前栈指针的值
	CBZ	  R0,Pro_Switch			;跳到下文切换
	
	;保存上文
	STMDB R0!,{R4-R11}
	;保存栈指针，到当前栈，用于下次任务切换使用
	LDR	  R1,=_MK_Current_Pro_	;将指向当前的进程控制块的地址加载到R1
	LDR	  R1,[R1]				;将当前的进程控制块地址指向的值加载到R1
	STR	  R0,[R1]				;将当前的进程控制块地址指向的值当作地址，然后往这个以R1的值为地址填入R0的值
	
	ENDP
	
Pro_Switch PROC
	;下文切换
	LDR	  R0,=_MK_Current_Pro_
	LDR   R1,=_MK_Next_Pro_
	LDR   R2,[R1]
	STR	  R2,[R0]
	
	LDR	  R0,[R2]
	LDMIA R0!,{R4-R11}			;将存储器的数据以R0开始地址的往后地址写回到R4到R11寄存器中
	MSR	  PSP,R0
	ORR	  LR,LR,#0x04			;是退出中断之后使用PSP堆栈
	BX    LR
	NOP
	ENDP
	
	NOP
	END
	
