	# print abcde
	push 65 # a
#loop:
	dup
	emit
	inc
	dup
	push 69
	sub
	jlt loo
	drop
	halt
