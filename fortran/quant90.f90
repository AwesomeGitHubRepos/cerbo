program quant90
use maff
implicit none

!character:: filename = "/home/mcarter/.fortran/itrk_l.h5"
character(len=10):: symbol
double precision:: opn, high, low, aclose
double precision, dimension(10000):: clsprice, rsi14, sma20, sma50, sma200, years
integer, dimension(10000):: vol, days, days8
character (len=10), dimension(10000)::dstamp
integer::i, j, nrows, y, m, d, ic
logical::aug_h5 ! set to true if you want to update the HDF5 file
logical:: symbol_arg
character(len=1024):: line, cmd

! needed for the 200dMA experiemtn
integer:: cross
double precision:: p

namelist /config/ symbol, symbol_arg, aug_h5


! read configuration file
open(unit=10, file = "hdf5ex.txt")
read(10, nml = config)
print *, "Get symbol from cmd line arg: ", symbol_arg

print *, "Writing HDF5 file: ", aug_h5
close(10)
!stop 0

if(symbol_arg) call getarg(1, symbol)

! convert to uppercase
! TODO save to a module
do i=1,10
   ic = ichar(symbol(i:i))
   if(ic>=97 .and. ic < 123) symbol(i:i) = char(ic-32)
   if(ic .eq. 0) symbol(i:i) = ' '
end do

print *, "Processing symbol: ", symbol

! read Yahoo download file
call chdir("/home/mcarter/.fortran")
open(unit=10, file = trim(symbol))
read(10,*) line
do i=1,10000
   read(10,*, end = 1099) dstamp(i), opn, high, low, clsprice(i), vol(i), aclose
enddo
stop 666 ! should reach here
1099 continue
close(10)
nrows = i -1


! perform calculations
call calc_rsi(clsprice, nrows, 14, rsi14)
call calc_sma(clsprice, nrows, 20, sma20)
call calc_sma(clsprice, nrows, 50, sma50)
call calc_sma(clsprice, nrows, 200, sma200)



!!$!dump data
!!$open(unit =11, file = trim(symbol) // ".dat")
!!$write(11,*) "#idx dstamp closing vol rsi14 sma20 sma50 sma200"
!!$do i = 1,nrows
!!$   write(11, "(A, I6,A11,F8.3)", advance = "no") "D", i, dstamp(i), clsprice(i)
!!$   write(11, "(I11)", advance = 'no') vol(i)
!!$   write(11, "(4F8.3)") rsi14(i), sma20(i), sma50(i), sma200(i)
!!$enddo
!!$close(11)

! create CDL file
open(unit=11, file = trim(symbol) // ".cdl")
write(11,*) "netcdf foo {"
write(11,*) ""
write(11,*) "   dimensions:"
write(11,*) "   nrows = ", nrows, ";"
write(11,*) ""
write(11,*) "   variables:"
write(11,*) "     double clsprice(nrows);"
write(11,*) "     int days(nrows);"
write(11,*) "     int days8(nrows);"
write(11,*) "     double rsi14(nrows);"
write(11,*) "     double sma20(nrows);"
write(11,*) "     double sma50(nrows);"
write(11,*) "     double sma200(nrows);"
write(11,*) "     int vol(nrows);"
write(11,*) "     double years(nrows);"
write(11,*) ""
write(11,*) '     days:units = "days starting 01-Jan-1900 =1";'
write(11,*) '     days8:units = "date in integer form YYYYMMDD";'
write(11,*) '     years:units = "date as a year float. E.g. 2014.0 is Jan 2014";'
write(11,*) ""
write(11,*) "   data:"
call write_cdl_farray("clsprice", "(F10.3)", clsprice, nrows)
do i=1,nrows
   call str2ymd(dstamp(i), y,m,d)
   days(i) = days1900(y,m,d)
   years(i) = year1900(y,m,d)
   days8(i) = int_date(y,m,d)
enddo
call write_cdl_iarray("days", "(I5)", days, nrows)
call write_cdl_iarray("days8", "(I8)", days8, nrows)
call write_cdl_farray("rsi14", "(F10.3)", rsi14, nrows)
call write_cdl_farray("sma20", "(F10.3)", sma20, nrows)
call write_cdl_farray("sma50", "(F10.3)", sma50, nrows)
call write_cdl_farray("sma200", "(F10.3)", sma200, nrows)
call write_cdl_iarray("vol", "(I10)", vol, nrows)
call write_cdl_farray("years", "(F10.3)", years, nrows)



write(11,*) "   }"
close(11)

! Create an NetCDF file from the CDL file
cmd = "ncgen -k 3 " // trim(symbol) // ".cdl"
call system(cmd)

!print *, "ND it's likely that the calcs are wrong due to input data being in wrong order"

print *, "200dma crossover"
do i = 200, nrows-200
   cross = 0
   p = clsprice(i)
   if(p.gt.sma200(i).and.clsprice(i-1).lt.sma200(i-1)) cross = 1
   if(p.lt.sma200(i).and.clsprice(i-1).gt.sma200(i-1)) cross = -1
   !cross = int(sgn(sma200(i) - clsprice(i)) - sgn(sma200(i-1) - clsprice(i-1)))
   if(cross.ne.0) then ! cross-over occurred
      low = clsprice(i)
      high = clsprice(i)
      do j = i, i+200
        low = min(low, clsprice(j))
         high = max(high, clsprice(j))
      enddo
      write(*,"(I6, A11,I3,5F9.3)") i, dstamp(i), cross, clsprice(i), low, high, low/clsprice(i), high/clsprice(i)
   endif
enddo

print *, "Finished"

end program quant90



subroutine write_cdl_farray(name, fmt, arr, nrows)
  implicit none
  character (len = *), intent(in):: name, fmt
  double precision, dimension(nrows):: arr
  integer, intent(in):: nrows

  integer:: i
  write(11,"(A,A,A)", advance = 'no') "     ", name, " = "
  do i = 1, nrows
     write(11, fmt, advance = 'no') arr(i)     
     if(i.lt.nrows) write(11, "(A)", advance = 'no') ", "
     if(modulo(i, 6).eq.0) write(11,*)
  enddo
  write(11,*) ";"
  write(11,*), ""
end subroutine write_cdl_farray

subroutine write_cdl_iarray(name, fmt, arr, nrows)
  implicit none
  character (len = *), intent(in):: name, fmt
  integer, dimension(nrows):: arr
  integer, intent(in):: nrows

  integer:: i
  write(11,"(A,A,A)", advance = 'no') "     ", name, " = "
  do i = 1, nrows
     write(11, fmt, advance = 'no') arr(i)     
     if(i.lt.nrows) write(11, "(A)", advance = 'no') ", "
     if(modulo(i, 6).eq.0) write(11,*)
  enddo
  write(11,*) ";"
  write(11,*), ""
end subroutine write_cdl_iarray
