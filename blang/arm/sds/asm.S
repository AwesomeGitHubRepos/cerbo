	.global main
main:
	push	{ip, lr}	@ entry point
	@ MAIN STUFF HERE

	@ END MAIN
	@ exit and cleanup
	mov	r0, #0 @ return value 0
	pop	{ip, pc}


