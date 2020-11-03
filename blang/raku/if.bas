print "should print 1"
if 2 then
	print 1
	if 3 then 
		print "nothing further should be printed"
	fi
fi

if 0 then
	print "this should never be printed"
fi
