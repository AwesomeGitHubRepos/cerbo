!#define globs common /vars/ m
module rubik
        implicit none
integer*1 m(6, 3, 3) ! face, row, col
integer     f, r, c
character :: colours(6) = (/ "W", "G", "R", "B", "O", "Y" /)

contains

subroutine init()
        do f=1, 6
        m(f,1:3, 1:3) = f
        end do
        !call prmat()
end subroutine        

subroutine prmat(coloured)
! print matrix
logical, optional :: coloured

logical :: coloured_
coloured_ = .false.
if(present(coloured)) coloured_ = coloured

do r=1, 3
do f=1, 6
do c=1, 3

if(coloured_) then
write (*, "(a1, a)", advance="no") colours(m(f, r, c)), ' '
else
write (*, "(i1, a)", advance="no") m(f, r, c), ' '
end if

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
        tmp1 = m(5, 3:1:-1, 3)
        m(5, 1:3, 3) = m(1, 3:1:-1, 1)
        m(1, 1:3, 1) = m(3, 1:3, 1)
        m(3, 1:3, 1) = m(6, 1:3, 1)
        m(6, 1:3, 1) = tmp1
end subroutine


subroutine rot3()
        ! rotate about the 
        integer*1 tmp1(3)
        ! edges
        tmp1 = m(1, 3, 3:1:-1)
        m(1, 3, 1:3) = m(4, 1:3, 1)
        m(4, 1:3, 1) = m(6, 1, 3:1:-1)
        m(6, 1, 1:3) = m(2, 1:3, 3)
        m(2, 1:3, 3) = tmp1
end subroutine

subroutine rot4()
        ! rotate about the 
        integer*1 tmp1(3)
        ! edges
        tmp1 = m(3, 1:3, 3)
        m(3, 1:3, 3) = m(1, 1:3, 3)
        m(1, 1:3, 3) = m(5, 3:1:-1, 1)
        m(5, 1:3, 1) = m(6, 3:1:-1, 3)
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
        integer :: face
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

subroutine test_basic_rot()
call test_rotn(1)
call test_rotn(2)
call test_rotn(3)
call test_rotn(4)
call test_rotn(5)
call test_rotn(6)
end subroutine

subroutine apply_rotations(rots)
        integer:: rots(:)
        integer :: n, i
        n = size(rots)
        do i=1, n
        call rotn(rots(i))
        enddo
end  subroutine

subroutine test_rotations(rots)
        integer:: rots(:)
        call init()
        call apply_rotations(rots)
        call prmat(.true.)
end subroutine

end module rubik

program cube
        use rubik

        ! layout :
        !      5        O
        !   2  1 4    G W B 
        !      3        R
        !      6        Y
        ! 1 W white
        ! 2 G green
        ! 3 R red
        ! 4 B blue
        ! 5 O orange
        ! 6 Y yellow

!integer f ! face [1..6]
!common /globs/ m

call init()

!call test_basic_rot()


call test_rotations((/1 ,2, 3, 4, 5, 6 /))
call test_rotations((/5, 1, 1, 4, 2, 4, 2, 3, 6, 1/))

end program

