' prog14.bas
' created 12-Mar-2018

def foo(x)
	if(x<10) then
		print(x)
		let x := x+1
		foo(x)
	fi
fed

foo(0)

		
