' prog07.bas
' test of if .. then .. else .. fi
' added 14-aug-2017

' should print 2
if(5 >6) then
	print(1)
else
	if(10<11) then
		print(2)
	else
		print(3)
	fi
fi

' should print 13
if 20 > (10+1) then
	print(13)
fi
