! Passing a dynamic array to a function/subroutine
! Known working on 05-Feb-2012
program ex01

real, allocatable :: a(:)

allocate(a(3))
a(1) = 1.1
a(2) = 2.2
a(3) = 3.3

write(*,*) total(a) ! prints out 6.6, as expected

contains
real function total (v)
  real,allocatable :: v(:)
  integer i
  total = 0.0
  do i = 1, size(v)
     total = total + v(i)
  end do
end function total


end program ex01
