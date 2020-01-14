0 prompt


defer foo
  z" Expect: DEFER not set" type cr 
\ defer bar
\ 1 . 
\ bar
' foo dup . .s dup dup dup .s execute execute
\ ' foo . dup execute

2 .

: x ." hello from x" cr ;
' x is foo
\ foo 


 : y ." caoi from y" cr ;
 ' y is foo
 foo


