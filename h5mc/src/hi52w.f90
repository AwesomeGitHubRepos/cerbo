program hi52w
  !use fgsl
  !use iso_c_binding
  !TODO this deserves some modularisation
  implicit none

  type rawtype
          character(len=10):: dstamp
          double precision:: opn, high, low, cls, vol, adjclose
  end type rawtype

  integer, parameter:: mrows = 3000
  TYPE(rawtype), dimension(1:mrows) :: raws

  integer  :: i, n,  lo, loc
  character (len=80) :: dummy
  !double precision:: rel
  

!!! inputs
  open(unit=11, file = "raw.dat", action = 'read')
  read(unit=11, fmt=*) dummy
  n = 0
  do
     n = n+1
     read(unit=11,fmt=*,  end=115) raws(n)
  enddo
115 continue
  n = n -1
  close(11)

!!! outputs  
  ! NB there are approx 261 trading days in a year
  do i = 262, n
        call write_pair(raws(i)%dstamp, raws(i)%cls)
        lo = max(1, i-261)

        loc = lo -1 + maxloc(raws(lo:i)%cls, dim =1)
        !rel = raws(loc)%cls/raws(i)%cls
        !call write_pair(raws(loc)%dstamp, raws(loc)%cls)
        !write(*, fmt="(F6.3, X, A3, L)") rel,  "HI-", (rel.eq.1)
        call write_trio(raws(loc)%dstamp, raws(loc)%cls, raws(i)%cls, "HI")
        loc = lo -1 + minloc(raws(lo:i)%cls, dim =1)
        call write_trio(raws(loc)%dstamp, raws(loc)%cls, raws(i)%cls, "LO")
        !write(*)
        print *

        !print *, raws(i)%dstamp, raws(i)%cls, ' ', raws(rmax)%dstamp, &
        !        & raws(rmax)%cls, rel, (rel.eq.1)
  enddo

!        call write_w52(maxloc, raws)

end 

subroutine write_pair(dstamp, price)
        character(len=10), intent(in) :: dstamp
        double precision, intent(in) :: price
        write(*, fmt="(A10, X, F7.2)", advance="no") dstamp, price
end subroutine

subroutine write_trio(dstamp, cls_loc, cls_i, prefix)
        implicit none
        character(len=10), intent(in) :: dstamp
        character(len=2), intent(in) :: prefix
        double precision, intent(in) :: cls_loc, cls_i

        double precision :: rel
        rel = cls_loc/cls_i
        write(*, fmt="(4X)", advance = "no")
        call write_pair(dstamp, cls_loc)
        write(*, fmt="(F6.3, X, A2, A, L)", advance = "no") rel,  prefix, "-" , (rel.eq.1)
end subroutine

!subroutine write52(raws, i, loc)
!        rel = raws(loc)%cls/raws(i)%cls
!        call write_pair(raws(loc)%dstamp, raws(loc)%cls)
!        write(*, fmt="(F6.3, X, A3, L)") rel,  "HI-", (rel.eq.1)
!end subroutine

