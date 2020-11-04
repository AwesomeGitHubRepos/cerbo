	# print ABCD
	push 65 # A
loop:
	#call hello
	dup
	call emit
	inc
	dup
	push 69
	sub
	jlt loop
	drop
