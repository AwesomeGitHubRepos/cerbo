print "Expect sub 1a,1b,2:"
gosub one
gosub two
halt

one:
	print "sub 1a"
	print "sub 1b"
	return

two:
	print "sub 2"
	return
