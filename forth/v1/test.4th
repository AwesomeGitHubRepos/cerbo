1 .  1 : t1 1+ 1+ 1+ ; 	t1 .
2 .  0 : t2 t1 t1 ; 	t2 .
3 .  0 : t3 t2 ;	t3 .
4 .  0 ' t3 execute .
5 . variable bar 5 bar ! bar @ .
6 . : t6 5 [ here ] dup . 1 - dup ?branch [ , ] ; t6
