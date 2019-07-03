program db117
      implicit none
      double precision pi, fc, dt
      parameter (pi=3.141593, fc = 300.0, dt = 1.0/80000.0)
     double precision vc ! voltage across capacitor
     double precision vt ! voltage input from battery
     character:: b
     real:: r ! random number
      integer i, j, s, secs
      integer raw
      parameter (raw = 10)
      parameter (secs = 60)
      double precision a
      a = 2.0 * pi * fc

      open(unit = raw, file='db05-117.raw', form='unformatted', &
              status='replace', access='stream')

      vt = 3.3 ! volts
      vc = 0
      !print*, "$data << EOD"
      do s = 1, secs
      do i = 1, 8000 ! 1 second 
      do j= 1, 10 ! subdivisions

      call RANDOM_NUMBER(r)
      !if(r.lt.0.5) then vt = 0 else vt = 3.3
      vt = merge(0.0, 3.3, r.lt.0.5)
      vc = vc + a *(vt - vc) * dt
      end do
      print*, vc
      b = char(int(vc*255.0/3.3))
      !write(raw, fmt='(I1)') b
      write(raw) b

      !if( i.eq.9) vt = 0 ! turn off the voltage
      end do
      enddo
      !print*, "EOD"
      !print*, 'plot "$data"'
      close(raw)


end program

