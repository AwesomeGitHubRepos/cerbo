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
@ PROLOGUE BEGIN
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

	.macro init_kstr dest, src, len
	ldr r0, =\dest
	ldr r1, =\src
	str r1, [r0]
	mov r1, \len
	str r1, [r0, #4] 	@length
	str r1, [r0, #8] 	@capacity (same as length for fixed string)
	.endm
	
@ MACROS END

	.global main
	main:
	@ entry point
	push    {ip, lr}

	BL	kstr_init
@ PROLOGUE END
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
