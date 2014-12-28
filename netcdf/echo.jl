using NetCDF
println(transpose(ncread("test.nc", "mystrings")))

println(ncread("test.nc", "myfloats"))
