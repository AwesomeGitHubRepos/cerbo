program vol
! volatility of a share as per DB04/325
implicit none
real a,b, lo, hi, v

print *, 'Enter high and low values, any order. "q" to quit'
100 continue
read (*,*) a, b
lo = b
hi = b
if (a.lt.b) then
	lo=a 
else
	hi=a
end if
v = abs(hi-lo)/(hi+lo)
print *, "vol = ", v, "hi/lo=", hi/lo
goto 100
end program
