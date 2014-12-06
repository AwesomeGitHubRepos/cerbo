program gnpoc
real :: vals(3, 5, 31), avg

i = 0
sum = 0
100 continue

read (*,*, ERR=400, END=400) vals(:, : , i+1)
i = i+1
do j = 1, 5
write (*, FMT=300) i, vals(:, j, i)
300 format (I3,3F7.2)
if(diff(vals(2, j, i)-vals(1, j, i), 0.04)) goto 200
if(diff((vals(1, j, i)+ vals(2, j, i))/2, vals(3, j, i))) goto 200

end do



!end do

goto 100

400 continue
print *, "num = ", i
do j = 1,5
!avg = sum(vals(3, j, 1:i))/i

total = 0.0
do k = 1, i
total = total + vals(3, j, k)
end do
print *, "mean ", j,  " = ", total/i
end do

stop

200 continue
print *, "fail!"

contains

logical function diff(x, y)
real :: x, y
diff = (abs(x-y).gt. 0.001)
end function

end program
