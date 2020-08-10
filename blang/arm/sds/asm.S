.macro OK
	LDR 	r0, =str4
	BL	puts
.endm

	.global main
main:
	push	{ip, lr}	@ entry point
	@ MAIN STUFF HERE

	LDR	r0, =str3
	BL	puts
	OK @ 1

	
	@ initialise a string
	LDR 	r4, =str1
	BL	sdsempty
	str	r0, [r4]
	OK @ 2
	
	@ set value of string
	LDR	r0, =str1
	LDR	r1, =str2
	BL	sdscpy
	LDR	r4, =str1
	STR	r0, [r4]
	OK @3

	LDR	r0, =str1
	BL	puts

	@ END MAIN
	@ exit and cleanup
	mov	r0, #0 @ return value 0
	pop	{ip, pc}

.data

.balign 4
str1: .word 0
str2: .asciz "Hello world\n"
str3: .asciz "Begin tests\n"
str4: .asciz "OK"
