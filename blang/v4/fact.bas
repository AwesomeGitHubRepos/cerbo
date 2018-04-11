def fact(n)
	if(n<1) then
		let v := 1
	else
		let v := n * fact(n -1)
	fi
	v
fed

print("10!=", fact(10), "should be 3628800")
