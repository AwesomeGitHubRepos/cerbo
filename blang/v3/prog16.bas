' prog16.bas
' created 12-Mar-2018

def fact(n)
	if n > 0 
	then
		let x := n * fact(n-1)
	else
		let x := 1
	fi
	x
fed

print("fact(10)=", fact(10))
