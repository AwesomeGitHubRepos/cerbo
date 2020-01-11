@ `cat' in assembler

.global main
.func main
main:
	@ prologue
    	push    {ip, lr}

	@ set up counter
	ldr	r1, =one
	vldr	s8, [r1] @ contains the value 1.0
	vsub.f32 s7, s7, s7 @ count

	ldr	r1, =values
	ldr	r4, =string
	vsub.f32 s0, s0, s0

	@mov	r6, #1 
	@vmov	s6, r6 @ increment
	@vmov.f32 s6, #1	
loop:
	@add	r6, r6, #1
	vadd.f32 s7, s7, s8
	vldr	s14, [r1]
	vadd.f32 s0, s0, s14
	add	r1, r1, #4
	cmp	r1, r4
	bne	loop

	vdiv.f32 s0, s0, s7

	bl 	print


	@ Exit from 'main'. This is like 'return 0' in C.
    	mov     r0, #0    @ Return 0.
    	pop     {ip, pc} 

print:
	push {lr}
	vcvt.f64.f32 d5, s0
	ldr r0, =string
	vmov r1, r2, d5
	bl printf
	pop {pc}

addr_values:
	.word values

.data

one: .float 1.0 @ don't change this
values: .float 3.8, -1.4, -0.1, 0.7, -0.2
string: .asciz "Average is: %f\n"
