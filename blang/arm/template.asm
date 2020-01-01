for-loop
	@ FOR
	@ for:from precalc
	{{from}}
	store r0, {{var}}

	@ for:to precalc
	{{to}}
	store r0, {{to-label}}

	@ for:test
{{for-test}}:
	load r0, {{var}}
	load r1, {{to-label}}
	cmp  r0, r1
	bgt {{end-for}}

	{{stmts}}

	@ for:next
	load r0, {{var}}
	add r0, r0, #1
	store r0, {{var}}
	b {{for-test}}
{{end-for}}:	@for:end

%%
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
