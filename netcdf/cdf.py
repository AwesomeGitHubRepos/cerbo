import numpy as np
import netCDF4 as nc

f = [10.0, np.nan, 12.0]
farr = np.array(f, dtype = "float64")
s = ["how", "now", "brown cows too long"]
sarr = nc.stringtochar(np.array(s, dtype = "S10"))

root = nc.Dataset('test.nc', 'w', format ='NETCDF4')
npoints = root.createDimension('npoints', 3)

nc_f = root.createVariable("myfloats",
                           datatype = "f8", dimensions = ('npoints',))
nc_f[:] = f
strlen = root.createDimension('strlen', 10)
nc_s = root.createVariable("mystrings", 
                           datatype = "S1", dimensions = ('npoints', 'strlen'))
nc_s[:] = sarr
nc.Dataset.close(root)
