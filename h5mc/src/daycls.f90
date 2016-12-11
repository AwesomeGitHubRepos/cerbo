program daycls
  !use fgsl
  !use iso_c_binding
  implicit none

  type rawtype
          character(len=10):: dstamp
          double precision:: opn, high, low, cls, vol, adjclose
  end type rawtype

  integer, parameter:: mrows = 3000
  TYPE(rawtype), dimension(1:mrows) :: raws

  integer  :: i, n, rmax, lo
  character (len=80) :: dummy
  double precision :: pcchg
  

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
  do i = 2, n
        pcchg = 100*(raws(i)%cls / raws(i-1)%cls-1)
        write(*, fmt="(A10, X, F10.2, X, F6.2)", advance="no") raws(i)%dstamp, raws(i)%cls, pcchg
        write(*, fmt="(X, A4, L, X, A4, L)") "GUP_", (pcchg > 10), "GDN_", (pcchg < -10)
  enddo


end program daycls
