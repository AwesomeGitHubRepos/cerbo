program indy

  parameter (mrows = 5000)
  character(20) :: s(mrows)
  integer indices(mrows), counts(mrows)

    read(*,*) nrows
    read(*,*) ncols
    do 100 i=1,nrows
       read(*,*) s(i)
100    continue

    call index(nrows, same, indices)
    call freq(nrows, indices, counts)
    do 200 i=1, nrows
       print *, s(i), indices(i), counts(i)
200    continue

contains
    function same(i,j) 
      logical same
      same = s(i).eq.s(j)
    end function same


end program




subroutine index(n, same, indices)
  logical same
  integer indices(n)
  !idx = 1
  indices(1) = 1
  do i=2,n
     do  j = 1,i
        indices(i) = j
        if (same(i,j))  goto 100
     end do
100 continue
end do
end subroutine

subroutine freq(n, inp, outp)
  integer inp(n), outp(n)
  do i =1,n
     outp(i) = 0
     do j = 1,n
        if (inp(i).eq.inp(j))  outp(i) = outp(i)+1
     end do
  end do
end subroutine
