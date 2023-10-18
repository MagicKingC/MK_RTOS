.text
.code 16
.global _CPU_InterruptEnable_
.global _CPU_InterruptDisable_
.global _MK_SR_Save_
.global _MK_SR_Restore_

;开中断
_CPU_InterruptEnable_ PROC
	CPSIE   I
	BX      LR
	ENDP
	
;关中断
_CPU_InterruptDisable_ PROC
	CPSID   I
	BX      LR
	ENDP
		
;带临界保护的中断
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
