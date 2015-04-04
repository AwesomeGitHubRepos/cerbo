program report01
use fgsl

!real rs6(619)
!character(len=20) :: index(619), match

real(fgsl_double) :: vals(5000), subset(5000)
integer nvals, nsubset

integer i, lo, mid, hi

open(14, file = '/home/mcarter/.fortran/_count')
read(14, *) nvals
open(15, file = '/home/mcarter/.fortran/PER', status='old')
open(16, file = '/home/mcarter/.fortran/FTSE_Index')

!nall = 0
do i=1,nall
	read(UNIT=15, FMT=*) val(nvals)
	!nall = nall +1
	!write(*,*) all(nall)
	! nall = i
enddo

fgsl_sort(vals, 1, nvals)


!999 continue


!read(15, *) rs6
!open(16, file = '/home/mcarter/.fortran/FTSE_Index', status='old')
!read(16, *) index

!match = "FTSE100"
!match = "AIM"
!lo = 0
!mid = 0
!hi = 0
!do i= 1, 619
!   if(index(i).ne. "FTSE100".and.index(i).ne."AIM") then
!      !print*,  I, index(i)
!      if(rs6(i).lt.-3.74) then
!         lo = lo +1
!      else if (rs6(i).gt.30.79) then
!         hi = hi + 1
!      else
!         mid = mid + 1
!      endif
!
!   endif
!enddo
!
!print *, lo, mid, hi
end program
