!#define globs common /vars/ m
module rubik
        implicit none
integer*1 m(6, 3, 3) ! face, row, col
integer     f, r, c

contains

subroutine init()
        do f=1, 6
        m(f,1:3, 1:3) = f
        end do
        !call prmat()
end subroutine        

subroutine prmat()
! print matrix
!use globs
do r=1, 3
do f=1, 6
do c=1, 3
write (*, "(i1, a)", advance="no") m(f, r, c), ' '
enddo
write (*, "(a2)", advance="no") '| '
enddo
write(*,*)
enddo
write(*,*)
end subroutine

subroutine rotf(f)
        ! rotate a face only
        integer f
        integer*1 m1(3, 3), tmp
        m1 = m(f, :, :)
        ! flip vertically
        do r=1,3
                tmp = m1(r, 1)
                m1(r, 1) = m1(r, 3)
                m1(r, 3) = tmp
        enddo
        m(f, :, :) = transpose(m1)
end subroutine 

subroutine rotn(f)
        ! rotate about face number f
        integer f
        call rotf(f)
        select case (f)
        case (1) 
                call rot1()
        case (2) 
                call rot2()
        case (3) 
                call rot3()
        case (4) 
                call rot4()
        case (5) 
                call rot5()
        case (6) 
                call rot6()
        case default 
                write(*,*) "ERR: rotn called with face ", f
        end select
end subroutine        

subroutine rot1()
        ! rotate about the 1 face
        integer*1 tmp1(3)
        ! edges
        tmp1 = m(5, 1, 1:3)
        m(5, 1, 1:3) = m(4, 1, 1:3)
        m(4, 1, 1:3) = m(3, 1, 1:3)
        m(3, 1, 1:3) = m(2, 1, 1:3)
        m(2, 1, 1:3) = tmp1

end subroutine

subroutine rot2()
        ! rotate about the 
        integer*1 tmp1(3)
        ! edges
        tmp1 = m(5, 1:3, 1)
        m(5, 1:3, 1) = m(1, 3:1:-1, 1)
        m(1, 1:3, 1) = m(3, 1:3, 1)
        m(3, 1:3, 1) = m(6, 3:1:-1, 1)
        m(6, 1:3, 1) = tmp1
end subroutine


subroutine rot3()
        ! rotate about the 
        integer*1 tmp1(3)
        ! edges
        tmp1 = m(1, 3, 3:1:-1)
        m(1, 3, 1:3) = m(4, 1:3, 1)
        m(4, 1:3, 1) = m(6, 1, 3:1:-1)
        m(6, 1, 1:3) = m(2, 3, 1:3)
        m(2, 1:3, 3) = tmp1
end subroutine

subroutine rot4()
        ! rotate about the 
        integer*1 tmp1(3)
        ! edges
        tmp1 = m(3, 1:3, 3)
        m(3, 1:3, 3) = m(1, 1:3, 3)
        m(1, 1:3, 3) = m(5, 1:3, 1)
        m(5, 1:3, 1) = m(6, 1:3, 3)
        m(6, 1:3, 3) = tmp1
end subroutine

subroutine rot5()
        ! rotate about the 
        integer*1 tmp1(3)
        ! edges
        tmp1 = m(4, 3:1:-1, 3)
        m(4, 1:3, 3) = m(1, 1, 1:3)
        m(1, 1, 1:3) = m(2, 3:1:-1, 1)
        m(2, 1:3, 1) = m(6, 3, 1:3)
        m(6, 3, 1:3) = tmp1
end subroutine

subroutine rot6()
        ! rotate about the 
        integer*1 tmp1(3)
        ! edges
        tmp1 = m(3, 3, 1:3)
        m(3, 3, 1:3) = m(4, 3, 1:3)
        m(4, 3, 1:3) = m(5, 3, 1:3)
        m(5, 3, 1:3) = m(2, 3, 1:3)
        m(2, 3, 1:3) = tmp1
end subroutine


subroutine test_rotn(face)
        integer face
       logical sane

       if(.true.) then
        call init()
else
        m(face, 1, 1:3) = (/1, 2, 3/)
        m(face , 2, 1:3) = (/4, 5, 6/)
        m(face, 3, 1:3) = (/7, 8, 9/)
endif
        !call prmat()
        call rotn(face)
        call prmat()

        sane = .true.
        do f=1,6
                sane = sane.and.(count(m.eq.1).eq.9)
        enddo
        !write(*, *) count(m.eq.1)
        write(*, *) 'sane: ', sane
end subroutine

end module rubik

program cube
        use rubik

        ! layout :
        !      5
        !   2  1 4
        !      3
        !      6

!integer f ! face [1..6]
!common /globs/ m

call init()

call test_rotn(1)
call test_rotn(2)
call test_rotn(3)
call test_rotn(4)
call test_rotn(5)
call test_rotn(6)
end program

