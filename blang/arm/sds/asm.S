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
	LDR	r0, [r0]
	BL	_sdsempty
	LDR 	r4, =str1
	str	r0, [r4]
	OK @ 2
	

	@ set value of string
	LDR	r0, =str1
	LDR	r0, [r0]
	LDR	r1, =str2
	@LDR	r1, [r1]
	BL	_sdscpy
	LDR	r4, =str1
	STR	r0, [r4]
	OK @3

	LDR	r0, =str1
	LDR	r0, [r0]
	BL	puts

	@ free it
	ldr	r0, =str1
	ldr	r0, [r0]
	bl _sdsfree
	OK @4

	@ free it double-free, Should barf
	ldr	r0, =str1
	ldr	r0, [r0]
	bl _sdsfree

exit:
@ END MAIN
	@ exit and cleanup
	mov	r0, #0 @ return value 0
	pop	{ip, pc}


@ SUPPORT FUNCTIONS

_strcat:
	stmdb sp!, {lr}
	bl strcat
	ldmia sp!, {pc}


_strcpy:
	stmdb sp!, {lr}
	bl strcpy
	ldmia sp!, {pc}


_malloc:
	stmdb sp!, {lr}
	bl malloc
	ldmia sp!, {pc}

	
_puts:
	stmdb sp!, {lr}
	bl puts
	ldmia sp!, {pc}

_sdsempty:
	stmdb sp!, {lr}
	bl sdsempty
	ldmia sp!, {pc}
	
_sdscpy:
	stmdb sp!, {lr}
	bl sdscpy
	ldmia sp!, {pc}

_sdsfree:
	stmdb sp!, {lr}
	bl sdsfree
	ldmia sp!, {pc}

.data

.balign 4
str1: .word 0 
str2: .asciz "Test of string copy"
str3: .asciz "Begin tests"
str4: .asciz "OK"
str5_fixed: .asciz "This is a fixed string"
str5: .space 32, 0
