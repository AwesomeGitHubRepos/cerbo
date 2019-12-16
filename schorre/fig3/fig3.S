@ following instructions need to be implemented
@ in order for Meta II to compile itself:
@ BF
@ BT
@ CI
@ CL
@ END
@ ID
@ GN1
@ GN2
@ LB
@ NUM
@ OUT
@ R
@ SET
@ SR
@ TST

.macro BLK n
.space (8*\n), 0
.endm

@put a given number at the top of the stack
.macro LDL n
	LDR R0, =stack
	MOV R1, \n
	STR R1, [R0]
.endm

@ store the number which is on top of the stack
@ put the addres AAA and pop the stack
.macro ST addr
	LDR R0, =stack
	LDR R1, =\addr
	STR R0, [R1]
.endm
	


.text
.global _start
_start:
	B A01
X:
	BLK 1
A01:
	LDL #0
	ST X

end:
	MOV R0, #0
	MOV R7, #1
	SWI 0	

.data
stack:
.word
.asciz "need to ort this out"

