@ `cat' in assembler

.global main
main:
	@ prologue
    	push    {ip, lr}


loop:
	bl	getchar
	cmp	r0, #-1 @ test for EOF
	@beq	end_loop
	addeq	pc, pc, #4
	blne	putchar
	b	loop
end_loop:


	@ Exit from 'main'. This is like 'return 0' in C.
    	mov     r0, #0    @ Return 0.
    	pop     {ip, pc} 

