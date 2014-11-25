! test calendrical functions
! see spreadsheet sheet test_days1900
program test
  use maff
  implicit none
  !write(*,*) days_in_month(2012, 2)
  integer y
  call test_days1900(1900, 1, 1, 1)
  call test_days1900(1960, 1, 1, 21916)
  call test_days1900(1960, 12, 31, 22281)
  call test_days1900(1965, 6, 23, 23916)
  call test_days1900(1970, 1, 1, 25569)
  call test_days1900(2012, 1, 1, 40909)
  call test_days1900(2012, 2, 1, 40940)
  call test_days1900(2012, 2, 28, 40967)
  call test_days1900(2012, 2, 29, 40968)
  call test_days1900(2012, 3, 1,  40969)
  call test_days1900(2012, 3, 15, 40983)
  call test_days1900(2012, 4, 1,  41000)
  call test_days1900(2012, 11, 10,41223)
  call test_days1900(2012, 12, 31,41274)
  call test_days1900(2013, 1, 1, 41275)
  call test_days1900(2013, 2, 1, 41306)
  call test_days1900(2013, 2, 28, 41333)
  call test_days1900(2013, 3, 1,  41334)
  call test_days1900(2013, 3, 15, 41348)
  call test_days1900(2013, 4, 1,  41365)
  call test_days1900(2013, 11, 10,41588)
  call test_days1900(2013, 12, 31,41639)

  contains
  subroutine test_days1900(y, m, d, answer)
    integer :: y, m, d, answer, calc
    character(4) :: ok

    calc = days1900(y, m, d)
    ok = 'pass'
    if(calc .ne. answer) ok = 'fail'
    write(*,*) 'test_days1900', y,m,d, calc, answer, ok
  end subroutine test_days1900
end program test
