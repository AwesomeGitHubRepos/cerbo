! MAFF - Mark's Arithmetic Fortran Formulas

module maff
  implicit none
contains

  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  !! sundry mathematical routines

  logical function mod0(a, b)
    ! does a mod b = 0?
    integer :: a, b
    mod0 = .false.
    if(modulo(a,b) .eq. 0) mod0 = .true.
  end function mod0

  subroutine rebase_array(in, out)
    real, allocatable :: in(:), out(:)

    real m
    integer i
    m = minval(in)
    do i = 1, size(in)
       out(i) = in(i) - m
    end do
  end subroutine rebase_array

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  !! calendrical routines

  integer function days1900(y, m, d)
    !TODO test
    integer y, m, d
    
    integer y1
    y1 = y - 1900
    days1900 = y1 * 365
    days1900 = days1900 + floor( 1.0 + real(y1 - 1)/4.0)
    days1900 = days1900 + cumdays(y, m, d)
    
  end function days1900

  integer function days_in_month(y, m)
    !TODO test
    integer y, m

    integer days(12)
    days_in_month = days(m)
    if (leap(y) .and. m .eq. 2) days_in_month = 29
    data days / 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 /
  end function days_in_month


  integer function cumdays(y, m, d)
    ! 01-Jan returns the value 1
    !TODO test
    integer y, m, d

    integer days(12)
    cumdays = days(m) + d
    if (leap(y) .and. m .gt. 2) cumdays = cumdays +1
    data days /0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334/

  end function cumdays

  logical function leap(y)
    integer y
    !leap = .false.
    !if(mod0(y, 4)) then
     !  if(.not. mod0(y, 100)) then
      !    if(mod0(y, 400) then
!leap = .true.
!else
 !  leap = .false.
   
  !  leap =  y .eq. 4 * floor( real(y) /4)
    leap = mod0(y, 4) .and. ( (.not. mod0(y, 100)) .or. mod0(y, 400))
  end function leap

  integer function days_in_year(y)
    ! Return the number of days in a year
    integer y
    days_in_year = 365
    if(leap(y)) days_in_year = 366
  end function days_in_year

  real function year1900(y, m, d)
    ! Return a date as a fraction of years
    integer y, m, d
    year1900 = real(y) + real(cumdays(y, m, d) - 1) / real(days_in_year(y))
  end function year1900


!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  !! finanical routines

  real function xnpv(times, amounts, rate)
    ! calculate npv of times (in fractional years) based on discount rate
    ! e.g. rate = 20 means 20% discount rate

    real, allocatable :: times(:), amounts(:)
    real rate

    real, allocatable :: rebased_times(:)
    real r
    integer i

    allocate(rebased_times(size(times)))
    call rebase_array(times, rebased_times)
    xnpv = 0.0
    r = 1.0 + rate/100.0
    do i = 1, size(times)
       xnpv = xnpv + amounts(i)/(r ** rebased_times(i))
    end do
    deallocate(rebased_times)
  end function xnpv


  integer function xirr_crude(times, amounts)
    real, allocatable :: times(:), amounts(:)
    real :: abs_xirr(-99:99)
    integer i
    do i = -99, 99
       abs_xirr(i) = abs(xnpv(times, amounts, real(i)))
       !write(*,*) i, abs_xirr(i)
    end do
    xirr_crude = minloc(abs_xirr, dim = 1) - 100
  end function xirr_crude




end module maff
