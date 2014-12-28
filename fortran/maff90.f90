! test the maff routines
program maff90

use maff

print *, "days 1900: ..."
print *, "1900-01-01: ", days1900(1900,1,1)
print *, "2014-12-27: ", days1900(2014,12,27)

end program maff90
