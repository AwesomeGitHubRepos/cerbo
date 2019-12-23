@ output a parabola
@ mcarter 23-Dec-2019 started

.macro chr char
	mov r0, \char
	bl putchar
.endm

.global main
main:
	@ entry point
	push    {ip, lr}

	mov r4, #1 @ outer

outer_loop:
	@ calculate number of spaces required
	mul r5, r4, r4
	lsr r5, r5, #3 @ divide by 8
space_loop:
	subs r5, r5, #1
	ble next
	chr #32 @ space
	b space_loop
next:
	chr #42 @ *
	chr #10 @ newline

	add r4, r4, #1
	subs r5, r4, #23
	bne outer_loop

bye:
	mov     r0, #0    @ Return 0.
	pop     {ip, pc}
