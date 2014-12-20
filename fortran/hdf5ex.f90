! /projects/src/hdf5-1.8.14/hdf5/bin/h5fc hdf5ex.f90 -o hdf5ex

! this actually works, surprisingly enough

program hdf5ex
USE HDF5

character:: filename = "/home/mcarter/.fortran/itrk_l.h5"
integer(HID_T) :: file_id, dset_id, dspace_id, dataspace
integer*8:: npoints
integer:: error
double precision:: buff(10000)
!sd_id = sfstart(file_name, DFACC_READ)
integer(Hsize_t), dimension(1:2):: dims = (/10000, 1 /)

call h5open_f(error)
if(error.eq.-1)stop 0
call chdir("/home/mcarter/.fortran")
!CALL h5fopen_f (filename, H5F_ACC_RDWR_F, file_id, error)
!CALL h5fopen_f ("itrk_l.h5", H5F_ACC_RDONLY_F, file_id, error)
CALL h5fopen_f ("itrk_l.h5", H5F_ACC_RDWR_F, file_id, error)

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
print *, "values are:", buff(1:npoints)
CALL h5dclose_f(dset_id, error)
if(error.eq.-1)stop 7

! now let's get ambitious and write a dataset
dims(1) = npoints
buff = buff + 0.1
!call h5fcreate_simple_f(1, dims, dspace_id, error)
call h5dcreate_f(file_id, "junk", H5T_NATIVE_DOUBLE, dspace_id, dset_id, error)
if(error.eq.01)stop 11
call h5dwrite_f(dset_id, H5T_NATIVE_DOUBLE, buff, dims, error)
if(error.eq.-1)stop 10
call h5dclose_f(dset_id, error)
if(error.eq.-1)stop 12

CALL h5fclose_f(file_id, error)
if(error.eq.-1)stop 8
CALL h5close_f(error)
if(error.eq.-1)stop 9

print *, "it compiles"

end program 
