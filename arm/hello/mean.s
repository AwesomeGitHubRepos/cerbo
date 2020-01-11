@ `cat' in assembler

.global main
.func main
main:
	@ prologue
    	push    {ip, lr}

	@ldr	r1, addr_values
	ldr	r1, =values
	vldr	s14, [r1]
	bl 	print


	@ Exit from 'main'. This is like 'return 0' in C.
    	mov     r0, #0    @ Return 0.
    	pop     {ip, pc} 

print:
	push {lr}
	vcvt.f64.f32 d5, s14
	ldr r0, =string
	vmov r1, r2, d5
	bl printf
	pop {pc}

addr_values:
	.word values

.data

values: .float 3.8, -1.4, -0.1, 0.7, -0.2
string: .asciz "Average is: %f\n"
