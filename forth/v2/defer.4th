0 prompt

' xdefer constant 'xdefer
: def <builds 'xdefer , does> @ execute ;
def baz
: s 1 . ;
' baz .name
' baz 8 +  @ .
' 'xdefer .
s ' is baz
baz

  variable  bar 
' xdefer bar !

: t 12 . cr ;
bar @ execute
 ' t bar ! 
\ 12 . cr
bar @ execute


." -------------" cr
defer foo
\ foo
\ ' foo . cr
 : s 666 . cr ;
\ s
foo
foo

' hi is foo
foo
foo

\  z" Expect: DEFER not set" type cr 

\ defer bar
\ 1 . 
\ bar
\ ' foo dup . .s dup   .s execute .s execute
\ ' foo . dup execute

2 .

: x ." hello from x" cr ;
' x is foo
 foo 


 : y ." caoi from y" cr ;
 ' y is foo
 foo


