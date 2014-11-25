! compile using gfortran -o recordjar recordjar.f90
! test using echo "Planet: Mercury" | recordjar
! conclusion: it doesn't work
program recordjar
character field(20), val(20)

read(*,*) field, val
write(*,*) "field =", field, "="
write(*,*) "value =", val, "="

end program recordjar
