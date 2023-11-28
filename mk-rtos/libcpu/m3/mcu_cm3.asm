	EXPORT _CPU_InterruptEnable_
	EXPORT _CPU_InterruptDisable_
	EXPORT _MK_SR_Save_
	EXPORT _MK_SR_Restore_
		
	AREA |.text|, CODE, READONLY, ALIGN=2
	THUMB
	REQUIRE8
	PRESERVE8
		
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
		
;���ٽ籣�����ж�
_MK_SR_Save_ PROC
	MRS		R0,PRIMASK
	CPSIE   I
	BX      LR
	ENDP

_MK_SR_Restore_ PROC
	MSR 	PRIMASK,R0
	BX      LR
	ENDP
		
	END
