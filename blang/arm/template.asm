prologue
	@ Automatically-generated assembler code

	@ macros

	.macro load reg, addr
	ldr \reg, =\addr
	ldr \reg, [\reg]
	.endm

	.macro store reg, addr
	push {r4}
	ldr r4, =\addr
	str \reg, [r4]
	pop {r4}
	.endm


	.global main
	main:
	@ entry point
	push    {ip, lr}
%%
epilogue
	@ exit and cleanup
	mov	r0, #0 @ return value 0
	pop	{ip, pc}

	@ FUNC: print integer
	@ IN: r0 integer to be printed
	printd:
	stmdb 	sp!, {lr}
	mov	r1, r0
	adr	r0, _printd
	bl 	printf
	ldmia	sp!, {pc}
	_printd:
	.asciz "Printing %d\n"	
	.balign 4

	@ FUNC: print string
	printstr:
	stmdb	sp!, {lr}
	bl	puts
	ldmia	sp!, {pc}
