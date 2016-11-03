! test the maff routines
! throughout, 'res' is assumed to mean known results
program maff90

use maff
implicit none

print *, "test of maff"


!print *, "days 1900: ..."
!print *, "1900-01-01: ", days1900(1900,1,1)
!print *, "2014-12-27: ", days1900(2014,12,27)

call test_dates
call test_i2ymd
call test_sortd
call test_quantile

contains

  subroutine test_i2ymd
    !use maff
    integer:: i, res, y, m, d
   
    print *, 'test_i2ymd - probably bugged'
    do i = 1, 800
       call i2ymd(i, y, m, d)
       !call date_int(res, y, m, d)
       !print *, 'foo', i, y, m, d, int_date(y, m, d)
    enddo
  end subroutine test_i2ymd
  
  subroutine test_dates
  use maff
  integer :: i, n
  integer :: y,m,d, ans, res
  logical :: ok
  ok = .true.
  !print *, "test_dates"
  open(unit = 24, file = 'xlsm/dates.txt')
  read(unit=24, fmt = *) n
  !print *, 'n=', n
  do i = 1, n
     read(unit=24, fmt = *) y, m, d, ans
     res = days1900(y, m, d)
     if(ans.ne.res) then
        !print *, y , m, d, ans, res, ans - res
        !stop
        ok = .false.
     endif
  enddo
  !print *, "pass"
  call assert(ok, 'test dates')
  
end subroutine test_dates

subroutine test_sortd
  use maff
  integer :: i, n
  double precision, allocatable :: arr(:), res(:) ! res is known output
  logical :: ok
  ok = .true.
  !print *, "test_sortd"
  open(unit = 24, file = 'xlsm/sort.txt')
  read(unit=24, fmt = *) n
  !print *, 'n=', n
  allocate(arr(n), res(n))
  !do i = 1, n
  read(unit=24, fmt=*) arr, res
  close(24)

  call sortd(arr)
  !print * , "s1"
  do i =1,n
     if(arr(i).ne.res(i)) ok = .false.
!        print *, "fail"
!        return
!     endif
  enddo
  ! print *, "pass"
  call assert(ok, "test_sortd")
  
     
end subroutine test_sortd


end program maff90

subroutine test_quantile
  use maff
  integer :: i, n
  double precision, allocatable :: arr(:), qs(:), res(:)
  double precision :: dummy, q
  logical :: ok
  !print *, "test_quantile"
  open(unit = 24, file = 'xlsm/random1.txt')
  read(unit=24, fmt = *) n
  !print *, 'n=', n
  allocate(arr(n))
  do i = 1, n
     read(unit=24, fmt=*) arr(i), dummy
  enddo
  close(24)

  call sortd(arr)
  !do i =1,n
  !   if(arr(i).ne.res(i)) then
  !      print *, "fail"
  !      return
  !   endif
  !enddo

  open(unit = 24, file = 'xlsm/quantile1.txt')
  read(unit=24, fmt = *) n
  allocate(qs(n), res(n))
  ok = .true.
  do i = 1, n
     read(unit=24, fmt = *) qs(i), res(i)
     q = quantile(arr, qs(i))
     !print *, res(i), q, tol4(res(i), q)
     if(.not.tol4(res(i),q)) ok = .false.
  enddo
  call assert(ok, "test_quantile")
  
     
end subroutine test_quantile

subroutine assert(ok, msg)
  logical :: ok
  character(len=*) :: msg
  character(len=4) :: res

  res = "FAIL"
  if(ok) res = "PASS"
  write(*,*) res, ' ', msg
end subroutine assert
