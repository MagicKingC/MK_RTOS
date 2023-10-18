.text
.code 16
.global _CPU_InterruptEnable_
.global _CPU_InterruptDisable_
.global _MK_SR_Save_
.global _MK_SR_Restore_

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
