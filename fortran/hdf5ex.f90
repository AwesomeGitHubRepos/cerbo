! /projects/src/hdf5-1.8.14/hdf5/bin/h5fc hdf5ex.f90 -o hdf5ex
! h5fc -g -fcheck=bounds hdf5ex.f90  -o hdf5ex

! this actually works, surprisingly enough

program hdf5ex
USE HDF5

character:: filename = "/home/mcarter/.fortran/itrk_l.h5"
character(len=100):: h5file
integer(HID_T) :: file_id, dset_id, dspace_id, dataspace
integer*8:: npoints
integer:: error
double precision:: buff(10000), rsi(10000)
!sd_id = sfstart(file_name, DFACC_READ)
integer(Hsize_t), dimension(1:2):: dims = (/10000, 1 /)


namelist /config/ h5file

open(unit=10, file = "hdf5ex.txt")
read(10, nml = config)
print *, "Processing file: ", h5file
close(10)
!stop 0

call h5open_f(error)
if(error.eq.-1)stop 0
call chdir("/home/mcarter/.fortran")
!CALL h5fopen_f (filename, H5F_ACC_RDWR_F, file_id, error)
!CALL h5fopen_f ("itrk_l.h5", H5F_ACC_RDONLY_F, file_id, error)
CALL h5fopen_f (h5file, H5F_ACC_RDWR_F, file_id, error)

if(error.eq.-1)stop 1
CALL h5dopen_f(file_id, "closing", dset_id, error)
if(error.eq.-1)stop 2
CALL h5dget_space_f(dset_id, dspace_id, error)
if(error.eq.01)stop 3
call h5sget_simple_extent_npoints_f(dspace_id, npoints, error)
if(error.eq.-1)stop 4
print *, "***********here"
print *, "Contains the following number of points:", npoints
if(npoints>10000)stop 5
CALL h5dread_f(dset_id, H5T_NATIVE_DOUBLE, buff, dims, error)
if(error.eq.-1)stop 6
!print *, "values are:", buff(1:npoints)
CALL h5dclose_f(dset_id, error)
if(error.eq.-1)stop 7

! now let's get ambitious and write a dataset
call calc_rsi(buff, npoints, 14, rsi)
dims(1) = npoints
buff = buff + 0.1
!call h5fcreate_simple_f(1, dims, dspace_id, error)
call h5dcreate_f(file_id, "rsi14", H5T_NATIVE_DOUBLE, dspace_id, dset_id, error)
if(error.eq.01)stop 11
call h5dwrite_f(dset_id, H5T_NATIVE_DOUBLE, rsi, dims, error)
if(error.eq.-1)stop 10
call h5dclose_f(dset_id, error)
if(error.eq.-1)stop 12

CALL h5fclose_f(file_id, error)
if(error.eq.-1)stop 8
CALL h5close_f(error)
if(error.eq.-1)stop 9

print *, "it compiles"

end program 

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
