! test financial functions
! see tacc/carali/tests/xirr-a.*
program ex03
  use maff
  implicit none

  integer xirr
  real, allocatable :: times(:), amounts(:)
  allocate(times(11))
  allocate(amounts(11))

  times(1) = year1900(2006, 1, 2)
  amounts(1) = -200.0
  times(2) = year1900(2006, 2, 1)
  amounts(2) = -100.0
  times(3) = year1900(2006, 3, 1)
  amounts(3) = -100.0
  times(4) = year1900(2006, 4, 1)
  amounts(4) = -100.0
  times(5) = year1900(2006, 5, 1)
  amounts(5) = -50.0
  times(6) = year1900(2006, 6, 1)
  amounts(6) = -100.0
  times(7) = year1900(2006, 7, 3)
  amounts(7) = -100.0
  times(8) = year1900(2006, 8, 1)
  amounts(8) = -100.0
  times(9) = year1900(2006, 9, 1)
  amounts(9) = -150.0
  times(10) = year1900(2006, 10, 2)
  amounts(10) = -100.0
  times(11) = year1900(2006, 10, 18)
  amounts(11) = 1247.0

  xirr = xirr_crude(times, amounts)
  write(*,*) "xirr =", xirr, ", s/b 32"
end program
