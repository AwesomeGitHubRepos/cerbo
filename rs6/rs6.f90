! exploration of relative strength
!make gfortran -fcheck=all -o rs6 rs6.f90



program rs6
implicit none
!parameter (mrows = 4000)
!real rs()
!integer num(-10:19)
!real rs6a(mrows)
!real rs6b(mrows), rs1y
!logical mask(mrows), prn
!integer nrows

!nrows = 0
!rs6a = -1000.0
!rs6b = 0.0

real v1y, v6a, v6b

do
   read(*,*) v6b, v1y ! rs6b, rs1y
   !if(v6b.eq.999) goto 100
   v6a = (v1y/100.0 + 1.0)/(v6b/100.0 + 1.0)*100.0 - 100.0
   !nrows = nrows + 1
   !rs6a(nrows) = v6a
   !rs6b(nrows) = v6b
   !prn = (50.le.v6a).and.(v6a.lt.60)
   !prn = .true.
   !if(prn) write(*,'(2F8.2)') rs6a(nrows), rs6b(nrows)
   !if(rs6a(nrows).gt.100) write(*,*) "^^^"
   write(*,*) v6a
end do
100 continue

end program
