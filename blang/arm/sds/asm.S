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
	mov	r1, #80 @default capacity of 80
	BL	_malloc
	LDR	r4, =str1
	str	r0, [r4]
	OK @ 2

	@ copy string: str1 = str2
	ldr	r0, =str1
	ldr	r1, =str2
	bl	_strcpy
	

	@ print copy
	ldr	r0, =str1
	bl	_puts


	@ test concat: str1 += str2
	ldr	r0, =str1
	ldr	r1, =str2
	bl	_strcat


	@ print copy
	ldr	r0, =str1
	bl	_puts

b exit	

	
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

.data

.balign 4
str1: .space 32, 0 
str2: .asciz "Test of string copy"
str3: .asciz "Begin tests"
str4: .asciz "OK"
str5_fixed: .asciz "This is a fixed string"
str5: .space 32, 0
