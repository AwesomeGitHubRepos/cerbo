.macro OK
	LDR 	r0, =str4
	BL	_puts
.endm

	.global main
main:
	push	{ip, lr}	@ entry point
	@ MAIN STUFF HERE

	@ display text 	
	LDR	r0, =str3
	BL	_puts
	OK @ 1

	
	@ initialise a string
	LDR	r0, =str1
	BL	_sdsempty
	LDR 	r4, =str1
	str	r0, [r4]
	OK @ 2
	
	@ set value of string
	LDR	r0, =str1
	LDR	r1, =str2
	BL	_sdscpy
	LDR	r4, =str1
	STR	r0, [r4]
	OK @3

	LDR	r0, =str1
	BL	puts

@ END MAIN
	@ exit and cleanup
	mov	r0, #0 @ return value 0
	pop	{ip, pc}


@ SUPPORT FUNCTIONS

_puts:
	stmfd sp!, {lr}
	bl puts
	ldmfd sp!, {pc}

_sdsempty:
	stmfd sp!, {lr}
	bl sdsempty
	ldmfd sp!, {pc}
	
_sdscpy:
	stmfd sp!, {lr}
	bl sdscpy
	ldmfd sp!, {pc}

.data

.balign 4
str1: .word 0
str2: .asciz "Hello world\n"
str3: .asciz "Begin tests\n"
str4: .asciz "OK"
