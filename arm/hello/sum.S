@ sum the first 5 positive integers (answer should be 15)
.global main
main:
	@ entry point
	push    {ip, lr}

	@ init
	mov 	r0, #5 @ loop variable
	mov 	r1, #0 @ result
loop:
	add 	r1, r1, r0
	subs	r0, r0, #1
	bgt	loop	

	@ print result
	ldr     r0, =message
	@ldr	r1, =cum
	@ldr	r1, [r1]
	bl      printf

	@ exit and claenup
	mov     r0, #0    @ Return 0.
	pop     {ip, pc}

    @ --------------------------------
    @ Data for the printf calls. The GNU assembler's ".asciz" directive
    @ automatically adds a NULL character termination.
message:
    .asciz "Sum is %d.\n"

cum:
	.word 0
count:
	.word 5
