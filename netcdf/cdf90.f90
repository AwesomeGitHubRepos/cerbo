! gfortran cdf90.f90 -I/usr/include  -lnetcdff

program cdf90
  use netcdf 
  implicit none

  character (len = *), parameter :: FILE_NAME = "test.nc"
  integer :: ncid, dimid, npoints, status, i, varid
  double precision:: myfloats(1000)
  character (len = 10), dimension(1000):: mystrings

  status = nf90_open(FILE_NAME, nf90_nowrite, ncid)

  status = nf90_inq_dimid(ncid, "npoints", dimid)
  status = nf90_inquire_dimension(ncid, dimid, len = npoints)
  print *, "Number of points: ", npoints


  ! read in myfloats
  status = nf90_inq_varid(ncid, "myfloats", varid)
  status = nf90_get_var(ncid, varid, myfloats(1:npoints))
  do i =1,npoints
     print *, myfloats(i)
  enddo

  ! now for the strings
  status = nf90_inq_varid(ncid, "mystrings", varid)
  status = nf90_get_var(ncid, varid, mystrings(1:npoints))
  do i =1,npoints
     print *, mystrings(i)
  enddo


end program cdf90
