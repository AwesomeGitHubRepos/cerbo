program quant90
use maff
implicit none

character:: filename = "/home/mcarter/.fortran/itrk_l.h5"
character(len=10):: symbol
double precision:: opn, high, low, aclose
double precision, dimension(10000):: clsprice, rsi14, sma20, sma50, sma200
integer, dimension(10000):: vol, days
character (len=10), dimension(10000)::dstamp
integer::i, j, nrows
logical::aug_h5 ! set to true if you want to update the HDF5 file
character(len=1024):: line

namelist /config/ symbol, aug_h5


! read configuration file
open(unit=10, file = "hdf5ex.txt")
read(10, nml = config)
print *, "Processing symbol: ", symbol
print *, "Writing HDF5 file: ", aug_h5
close(10)
!stop 0

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


!dump data
open(unit =11, file = trim(symbol) // ".dat")
write(11,*) "#idx dstamp closing vol rsi14 sma20 sma50 sma200"
do i = 1,nrows
   write(11, "(A, I6,A11,F8.3)", advance = "no") "D", i, dstamp(i), clsprice(i)
   write(11, "(I11)", advance = 'no') vol(i)
   write(11, "(4F8.3)") rsi14(i), sma20(i), sma50(i), sma200(i)
enddo
close(11)

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
write(11,*) "     double rsi14(nrows);"
write(11,*) "     double sma20(nrows);"
write(11,*) "     double sma50(nrows);"
write(11,*) "     double sma200(nrows);"
write(11,*) "     int vol(nrows);"
write(11,*) ""
write(11,*) '     days:units = "days starting 01-Jan-1900 =1";'
write(11,*) ""
write(11,*) "   data:"
call write_cdl_farray("clsprice", "(F10.3)", clsprice, nrows)
do i=1,nrows
 days(i) = str2days1900(dstamp(i))
enddo
call write_cdl_iarray("days", "(I5)", days, nrows)
call write_cdl_farray("rsi14", "(F10.3)", rsi14, nrows)
call write_cdl_farray("sma20", "(F10.3)", sma20, nrows)
call write_cdl_farray("sma50", "(F10.3)", sma50, nrows)
call write_cdl_farray("sma200", "(F10.3)", sma200, nrows)
call write_cdl_iarray("vol", "(I10)", vol, nrows)


write(11,*) "   }"
close(11)


print *, "ND it's likely that the calcs are wrong due to input data being in wrong order"
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
