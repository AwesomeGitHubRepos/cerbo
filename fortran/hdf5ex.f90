! /projects/src/hdf5-1.8.14/hdf5/bin/h5fc hdf5ex.f90 -o hdf5ex
! h5fc -g -fcheck=bounds hdf5ex.f90  -o hdf5ex

! this actually works, surprisingly enough

program hdf5ex
USE HDF5
implicit none

character:: filename = "/home/mcarter/.fortran/itrk_l.h5"
character(len=10):: symbol
integer(HID_T) :: file_id, dset_id, dspace_id, dataspace
integer*8:: npoints
integer:: error
double precision, dimension(10000):: closing, rsi14, sma20, sma50, sma200, vol
!sd_id = sfstart(file_name, DFACC_READ)
!integer(Hsize_t), dimension(1:2):: dims = (/10000, 1 /)
integer::dstamp, i
logical::aug_h5 ! set to true if you want to update the HDF5 file
integer(Hsize_t), dimension(1):: dims

namelist /config/ symbol, aug_h5

open(unit=10, file = "hdf5ex.txt")
read(10, nml = config)
print *, "Processing symbol: ", symbol
print *, "Writing HDF5 file: ", aug_h5
close(10)
!stop 0

call h5open_f(error)
if(error.eq.-1)stop 0
call chdir("/home/mcarter/.fortran")
!CALL h5fopen_f (filename, H5F_ACC_RDWR_F, file_id, error)
!CALL h5fopen_f ("itrk_l.h5", H5F_ACC_RDONLY_F, file_id, error)
CALL h5fopen_f (trim(symbol) // ".h5", H5F_ACC_RDWR_F, file_id, error)

if(error.eq.-1)stop 1
CALL h5dopen_f(file_id, "closing", dset_id, error)
if(error.eq.-1)stop 2
CALL h5dget_space_f(dset_id, dspace_id, error)
if(error.eq.01)stop 3
call h5sget_simple_extent_npoints_f(dspace_id, npoints, error)
if(error.eq.-1)stop 4
print *, "Contains the following number of points:", npoints
if(npoints>10000)stop 5
CALL h5dread_f(dset_id, H5T_NATIVE_DOUBLE, closing, dims, error)
if(error.eq.-1)stop 6
!print *, "values are:", buff(1:npoints)
CALL h5dclose_f(dset_id, error)
if(error.eq.-1)stop 7

call calc_rsi(closing, npoints, 14, rsi14)
call calc_sma(closing, npoints, 20, sma20)
call calc_sma(closing, npoints, 50, sma50)
call calc_sma(closing, npoints, 200, sma200)

if(aug_h5)then
   call write_h5_darray(file_id, dspace_id, rsi14, npoints, "rsi14")
   call write_h5_darray(file_id, dspace_id, sma20, npoints, "sma20")
   call write_h5_darray(file_id, dspace_id, sma50, npoints, "sma50")
   call write_h5_darray(file_id, dspace_id, sma200, npoints, "sma200")
endif

CALL h5fclose_f(file_id, error)
if(error.eq.-1)stop 8
CALL h5close_f(error)
if(error.eq.-1)stop 9

!dump data
open(unit =11, file = trim(symbol) // ".dat")
write(11,*) "#idx dstamp closing vol rsi14 sma20 sma50 sma200"
dstamp = 666 ! TODO fix
vol = 666 ! TODO fix
do i = 1,npoints
   write(11,"(I0,I10,6F8.3)") i, dstamp, closing(i), vol(i), rsi14(i), sma20(i), sma50(i), sma200(i)
enddo
close(11)

print *, "Finished"

end program 


subroutine write_h5_darray(file_id, dspace_id, arr, npoints, name)
  use HDF5
  integer(HID_T), intent(in) :: file_id
  integer(HID_T), intent(in) :: dspace_id
  integer*8, intent(in):: npoints
  double precision, intent(in), dimension(npoints):: arr
  character(len=*), intent(in):: name !name of the dataset


  integer(HID_T):: dset_id
  integer(Hsize_t), dimension(1):: dims
  integer:: error

  dims(1) = npoints

  call h5dcreate_f(file_id, name, H5T_NATIVE_DOUBLE, dspace_id, dset_id, error)
  if(error.eq.01)stop 501
  call h5dwrite_f(dset_id, H5T_NATIVE_DOUBLE, arr, dims, error)
  if(error.eq.-1)stop 502
  call h5dclose_f(dset_id, error)
  if(error.eq.-1)stop 503
end subroutine write_h5_darray


subroutine calc_rsi(prices, npoints, n, rsi)
  ! appears to be correct
  ! calculate the relative strength
  integer*8, intent(in) :: npoints
  double precision, intent(in), dimension(npoints):: prices
  integer, intent(in) :: n
  double precision, intent(out), dimension(npoints) ::   rsi

  double precision, dimension(npoints):: deltas
  double precision :: up, down
  integer*8::  i


  do i=1, npoints-1
     deltas(i) = prices(i+1) - prices(i)
  enddo

  up = 0
  down = 0
  do i = 1, min(n+1, npoints) ! default should be n+1
     up = up + dmax1(0., deltas(i))/n
     down = down + max(0., -deltas(i))/n
  enddo
  rs = up/down
  rsi = 0
  rsi(1:n) = 100.0 * rs/(1.0+rs)

  do i=n,npoints
     up = (up *(n -1.0) + max(0., deltas(i-1)))/n
     down = (down*(n-1.0) + max(0., -deltas(i-1)))/n
     rs = up/down
     rsi(i) = 100.0 * rs/(1.0+rs)
  enddo
end subroutine calc_rsi


subroutine calc_sma(prices, npoints, n, sma)
  !looks good
  ! calculate a simple moving average
  integer*8, intent(in) :: npoints
  double precision, intent(in), dimension(npoints):: prices
  integer, intent(in) :: n
  double precision, intent(out), dimension(npoints) ::  sma

  integer*8::  i, i0

  do i= 1, npoints
     i0 = max(1, 1+ i -n) ! cope with beginning points
     sma(i) = sum(prices(i0:i))/dble(i - i0+1)
  end do
end subroutine
