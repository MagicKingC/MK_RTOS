	EXPORT _TriggerPendSV_
	EXPORT _MK_RTOS_RUN_
	EXPORT _CPU_InterruptEnable_
	EXPORT _CPU_InterruptDisable_
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

;���ж�
_CPU_InterruptEnable_ PROC
	CPSIE   I
	BX      LR
	ENDP
	
;���ж�
_CPU_InterruptDisable_ PROC
	CPSID   I
	BX      LR
	ENDP
		
_MK_RTOS_RUN_ PROC

	;����pendsv���ȼ�
	LDR		R0,=NVIC_SYSPRI14
	LDR		R1,=NVIC_PENDSV_PRI
	STRB	R1,[R0]
	
	MOVS  R0,#0
	MSR	  PSP,R0

	;����pendsv�ж�
	LDR	  R0,=NVIC_INT_CTRL
	LDR	  R1,=NVIC_PENDSVSET
	STR   R1,[R0]
	
	CPSIE   I
	ENDP
		
_TriggerPendSV_ PROC
	;����pendsv�ж�
	LDR	  R0,=NVIC_INT_CTRL
	LDR	  R1,=NVIC_PENDSVSET
	STR   R1,[R0]
	ENDP
		
PendSV_Handler PROC
	;�ж�psp�Ƿ�Ϊ0��Ĭ��ϵͳ��ʼ��ʱ�Ὣpsp��ֵ��ֵΪ0
	MRS	  R0,PSP				;ȡ����ǰջָ���ֵ
	CBZ	  R0,Pro_Switch			;���������л�
	
	;��������
	STMDB R0!,{R4-R11}
	;����ջָ�룬����ǰջ�������´������л�ʹ��
	LDR	  R1,=_MK_Current_Pro_	;��ָ��ǰ�Ľ��̿��ƿ�ĵ�ַ���ص�R1
	LDR	  R1,[R1]				;����ǰ�Ľ��̿��ƿ��ַָ���ֵ���ص�R1
	STR	  R0,[R1]				;����ǰ�Ľ��̿��ƿ��ַָ���ֵ������ַ��Ȼ���������R1��ֵΪ��ַ����R0��ֵ
	
	ENDP
	
Pro_Switch PROC
	;�����л�
	LDR	  R0,=_MK_Current_Pro_
	LDR   R1,=_MK_Next_Pro_
	LDR   R2,[R1]
	STR	  R2,[R0]
	
	LDR	  R0,[R2]
	LDMIA R0!,{R4-R11}			;���洢����������R0��ʼ��ַ�������ַд�ص�R4��R11�Ĵ�����
	MSR	  PSP,R0
	ORR	  LR,LR,#0x04			;���˳��ж�֮��ʹ��PSP��ջ
	BX    LR
	NOP
	ENDP
	
	END
