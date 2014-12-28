program y2nc
! convert yahoo timedumps to nc file
use netcdf
implicit none

double precision:: opn, high, low, aclose
double precision, dimension(10000):: clsprice, vol
character (len=10), dimension(10000):: dstamp
character (len=80):: line
integer:: i, nrows, dimids(2), dims1(1)
integer:: status, dimid_dstamp, dimid_nrows, dstamp_varid, ncid, clsprice_varid

call chdir("/home/mcarter/.fortran")

! read all the data
open(unit = 10, file = "HYH", action = 'read')

read(10,*) line
do i=1,10000
   read(10,*, end = 1099) dstamp(i), opn, high, low, clsprice(i), vol(i), aclose
enddo
stop 666 ! should reach here
1099 continue
close(10)
nrows = i -1

! delete previous NC file
open(unit = 10, file = 'hyh.nc', action = 'write')
close(10, status = 'delete')

!write it to NC file
call checknc("create", nf90_create('hyh.nc', nf90_clobber, ncid))
!call checknc("open", nf90_open('hyh.nc', nf90_write, ncid))

!call checknc("nc close", nf90_close(ncid))
!call checknc("redef", nf90_redef(ncid))
call checknc("dim nrows",nf90_def_dim(ncid, "nrows", nrows, dimid_nrows))
call checknc("dim dstamp", nf90_def_dim(ncid, "str10", 10, dimid_dstamp))

!call checknc("dstamp varid", nf90_create("dstamp", nf90_clobber, dstamp_varid)) !, 10*nrows, nf90_sizehint_default, 
dimids = (/  dimid_dstamp , dimid_nrows /)
call checknc("create dstamp var", nf90_def_var(ncid, "dstamp", nf90_char, &
     dimids, dstamp_varid))
!call checknc("enddef dstamp", nf90_enddef(ncid))

dims1 = (/ dimid_nrows /)
call checknc("def var clsprice", nf90_def_var(ncid, "clsprice", nf90_double, &
     dims1, clsprice_varid))


call checknc("put_var dstamp", nf90_put_var(ncid, dstamp_varid, dstamp))
call checknc("put_var cls_price", nf90_put_var(ncid, clsprice_varid, clsprice))



call checknc("enddef", nf90_enddef(ncid))

print *, "Number of data rows:", nrows

print *, "dstamps:", dstamp(1:nrows)
print *, "volumes:", vol(1:nrows)
print *, "closing prices", clsprice(1:nrows)

call checknc("nc close", nf90_close(ncid))
end program y2nc

subroutine checknc(msg, status)
character(len=*):: msg
integer:: status

if(status /=nf90_NoErr) then
   print *, "Problem with NC handling:", msg
   !print *, nf90_strerror(status)
   stop 500
endif
end subroutine
