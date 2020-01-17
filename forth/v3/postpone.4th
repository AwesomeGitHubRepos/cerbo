0 prompt
: end postpone then ; immediate
\ see end
: x if 2 . end 3 . cr ;
\ see x
." Expect 3" cr
0 x
." Expect 2 3" cr
1 x

: ?again1 	postpone ?branch , ; immediate
\ see ?again1

: y 		5 begin dup . 1 - dup ?again1 drop cr ;
." Expect 5 4 3 2 1" cr
y

: ?again2	postpone ?again1 ; immediate
: y 		5 begin dup . 1 - dup ?again2 drop cr ;
." Expect 5 4 3 2 1" cr
y
