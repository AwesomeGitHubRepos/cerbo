0 prompt
: f  [ 1 2 + ] literal  ;
." Expect: 3 3" cr
f  . f  . cr

: g [ f 1 + ] literal . ;
." Expect: 4 4" cr
g g cr

: n23     23  postpone literal    ; immediate
: nn n23 . cr ;
." Expect 23:" cr
nn
." Expect 23:" cr
nn

.s cr
