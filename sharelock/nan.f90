program nan


real rs6(619)
character(len=20) :: index(619), match

integer i, lo, mid, hi

open(15, file = '/home/mcarter/.fortran/RS_6Month', status='old')
read(15, *) rs6
open(16, file = '/home/mcarter/.fortran/FTSE_Index', status='old')
read(16, *) index

match = "FTSE100"
!match = "AIM"
lo = 0
mid = 0
hi = 0
do i= 1, 619
   if(index(i).ne. "FTSE100".and.index(i).ne."AIM") then
      !print*,  I, index(i)
      if(rs6(i).lt.-3.74) then
         lo = lo +1
      else if (rs6(i).gt.30.79) then
         hi = hi + 1
      else
         mid = mid + 1
      endif

   endif
enddo

print *, lo, mid, hi
end program
