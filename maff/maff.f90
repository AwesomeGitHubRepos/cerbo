! MODULE: maff
! DESCRIPTION:
!> Mark's Algorithms For Fortran

module maff
implicit none
contains

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!! sundry mathematical routines

  !> @section sundry Sundry Mathematical routines

logical  function tol4(a, b)
    !within 4 decimal places
    double precision :: a, b
    tol4 = abs(a-b).le.(10.0d0**(-4.0d0))
  end function tol4
  
function modz(a, b)
  ! does a mod b = 0?
  integer :: a, b
  logical:: modz
  modz = .false.
  if(modulo(a,b) .eq. 0) modz = .true.
end function modz

subroutine rebase_array(in, out)
  real, allocatable :: in(:), out(:)
  
  real m
  integer i
  m = minval(in)
  do i = 1, size(in)
     out(i) = in(i) - m
  end do
end subroutine rebase_array

double precision function sgn(x)
  double precision, intent(in):: x
  sgn = 0.0
  if(x.gt.0.0) sgn = 1.0
  if(x.lt.0.0) sgn = -1.0
end function sgn

!> sortd sort doubles in place
!! Referenced from:
!! http://rosettacode.org/wiki/Sorting_algorithms/Insertion_sort#Fortran
SUBROUTINE sortd(a)
  double precision, INTENT(in out), DIMENSION(:) :: a
  double precision :: temp
  INTEGER :: i, j

  !print *, "size a = ", size(a)
  DO i = 2, SIZE(a)
     j = i - 1
     temp = a(i)
     !DO WHILE (j>=1 .AND. a(j)>temp)
     do
        if (j.lt.1) goto 100
        if(a(j).le.temp) goto 100
        a(j+1) = a(j)
        j = j - 1
     END DO
100  continue
     a(j+1) = temp
  END DO
END SUBROUTINE sortd
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!! statistical routines

!> calculates quantiles according to Excel interpolation formula
! https://en.wikipedia.org/wiki/Quantile
! assumes array is sorted

double precision function quantile(arr, q)
  double precision :: arr(:)
  double precision :: q

  double precision :: hd
  integer :: hi

  hd = (dble(size(arr))-1.0d0) * q + 1.0d0
  hi = floor(hd)
  if(q.eq.1.0d0) then
     quantile = arr(size(arr))
     return
  endif
  quantile = arr(hi) + (hd - dble(hi)) * (arr(hi+1) - arr(hi))
  !integer:: idx0
  !double precision :: didx0

  !didx0 = (dble(size(arr))-1.0d0) * q + 1.0d0
  !idx0 = ceiling(didx0)
  !if(idx0.eq.didx0) then
  !   quantile = arr(idx0)
  !   return
  !endif

  !v1 = arr(idx0)
  !v2 = arr(idx0+1)
  !quantile = v1 * ? + v2 *
  
end function quantile

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!! calendrical routines


!> leap year incrementing function
function leapinc(y)
  integer:: y
  integer:: leapinc
  logical:: m4, m100not, m400

  m4 = (modulo(y, 4).eq.0)
  m100not = (modulo(y, 100).ne.0)
  m400 = (modulo(y, 400).eq.0)
  leapinc = 0
  if(m4 .and. (m100not .or. m400)) leapinc = 1
end function leapinc


!> cumulative day array.
!! load arr with the cumulative days for the year
subroutine cda(y, arr)
  integer, intent(in):: y
  integer, intent(out) :: arr(12)

  integer :: days(12)
  arr = days
  if(leapinc(y).eq.1) arr(3:12) = 1 + arr(3:12)
  !if (m .gt. 2) cumdays = cumdays + leapinc(y)
  data days /0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334/
end subroutine cda
  


integer function cumdays(y, m, d)
  ! 01-Jan returns the value 1
  !TODO test
  !integer:: cumdays
  integer y, m, d
  
  integer days(12)

  call cda(y, days)
  cumdays = days(m) + d

  
end function cumdays


!> days since 01-Jan-1900, where
!! 1900-01-01 == 1
function days1900(y, m, d) !result(integer)
  !TODO test
  integer:: days1900 ! output
  integer, intent(in) ::  y, m, d
  
  integer y1
  y1 = y - 1900
  days1900 = y1 * 365
  days1900 = days1900 + floor( 1.0 + real(y1 - 1)/4.0)
  days1900 = days1900 + cumdays(y, m, d)
  
end function days1900

!> convert as serial number i to integers (y, m , d) 
!! TODO test
!! should split some of this functionality out
subroutine i2ymd(i, y, m, d)
  integer, intent(in):: i
  integer, intent(out):: y, m, d
  !integer :: i2ymd ! result

  integer :: i2, cum, cdays(12), j, by
  !i2 = i + floor(1900.0d0 * 365.25d0)-1!  + 475 ! 475 = putative leap years up to 1900
  y = floor(dble(i)/365.25) + 1900
  call cda(y, cdays)

  !by = days1900(y-1, 12, 31)!+1
  by = days1900(y-1, 1, 1)!+1
  cum = i - by
  !print *, cum, by
  do j = 1, 12
     if (cdays(j).lt.cum) m = j
  enddo
  d = cum - cdays(m)
  !i2ymd = int_date(y , m, d)  
end subroutine i2ymd

function days_in_month(y, m)
  !TODO test
  integer y, m
  integer:: days_in_month
  
  integer days(12)
  days_in_month = days(m)
  if (m .eq. 2) days_in_month = 28 + leapinc(y)
  data days / 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 /
end function days_in_month





integer function days_in_year(y)
  ! Return the number of days in a year
  integer y
  days_in_year = 365 + leapinc(y)
end function days_in_year

real function year1900(y, m, d)
  ! Return a date as a fraction of years
  integer y, m, d
  year1900 = real(y) + (real(cumdays(y, m, d)) - 1.0) / real(days_in_year(y))
end function year1900


subroutine str2ymd(str10, y, m, d)
  character(len=10), intent(in):: str10
  integer, intent(out):: y,m,d
  read (str10(1:4), "(I4)") y
  read (str10(6:7), "(I2)") m
  read (str10(9:10), "(I2)") d
end subroutine str2ymd

!> Convert YYYY-MM-DD to days since 1900
!! @param str string of form YYYY-MM-DD
integer function str2days1900(str)
  !convert YYYY-MM-DD to days since 1900
  !TODO test
  character (len=10)::str
  integer::y,m,d
  call str2ymd(str, y, m, d)
  str2days1900 = days1900(y,m,d)
end function str2days1900

!> Convert (y,m,d) into integer YYYYMMDD
integer function int_date(y, m, d)
  integer, intent(in):: y,m,d
  int_date = (y * 100 + m)*100 +d
end function int_date

!> Convert integer YYYYMMDD into (y,m, d)
!! TODO check. I think it might be wrong
subroutine date_int(i, y, m, d)
  integer, intent(in)::i
  integer, intent(out):: y, m, d

  double precision:: id
  id = dble(i)
  d = mod(i, 100)
  m = floor(dble(mod(i, 10000))/100.0)
  y = floor(dble(i)/10000.0)
  !id = id - y * 1000
  !m = floor((id - y * 1000) /100)
  !d = i - floor(id/100000) * 100000
end subroutine date_int

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!! finanical routines

! TODO uncomment this stuff later
!!$real function xnpv(times, amounts, rate)
!!$  ! calculate npv of times (in fractional years) based on discount rate
!!$  ! e.g. rate = 20 means 20% discount rate
!!$  
!!$  real, allocatable :: times(:), amounts(:)
!!$  real rate
!!$  
!!$  real, allocatable :: rebased_times(:)
!!$  real r
!!$  integer i
!!$  
!!$  allocate(rebased_times(size(times)))
!!$  call rebase_array(times, rebased_times)
!!$  xnpv = 0.0
!!$  r = 1.0 + rate/100.0
!!$  do i = 1, size(times)
!!$     xnpv = xnpv + amounts(i)/(r ** rebased_times(i))
!!$  end do
!!$  deallocate(rebased_times)
!!$end function xnpv


!!$integer function xirr_crude(times, amounts)
!!$  real, allocatable :: times(:), amounts(:)
!!$  real :: abs_xirr(-99:99)
!!$  integer i
!!$  do i = -99, 99
!!$     abs_xirr(i) = abs(xnpv(times, amounts, real(i)))
!!$     !write(*,*) i, abs_xirr(i)
!!$  end do
!!$  xirr_crude = minloc(abs_xirr, dim = 1) - 100
!!$end function xirr_crude




end module maff
