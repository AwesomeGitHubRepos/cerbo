! MODULE: maff
! DESCRIPTION:
!> Mark's Arithmetic Fortran Formulas

module maff
implicit none
contains

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!! sundry mathematical routines

  !> @section sundry Sundry Mathematical routines
  
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

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
!! calendrical routines


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


integer function cumdays(y, m, d)
  ! 01-Jan returns the value 1
  !TODO test
  !integer:: cumdays
  integer y, m, d
  
  integer days(12)
  cumdays = days(m) + d
  if (m .gt. 2) cumdays = cumdays + leapinc(y)
  data days /0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334/
  
end function cumdays



function days1900(y, m, d)
  !TODO test
  integer:: days1900
  integer y, m, d
  
  integer y1
  y1 = y - 1900
  days1900 = y1 * 365
  days1900 = days1900 + floor( 1.0 + real(y1 - 1)/4.0)
  days1900 = days1900 + cumdays(y, m, d)
  
end function days1900

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

integer function int_date(y, m, d)
  !convert (y,m,d) into integer YYYYMMDD
  integer, intent(in):: y,m,d
  int_date = (y * 100 + m)*100 +d
end function int_date


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
