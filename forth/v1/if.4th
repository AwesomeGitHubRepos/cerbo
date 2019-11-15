0 prompt
: .test cr type cr ;

z" vanilla if-then. Expect no output" .test
: t1 if 16 . then  ; 
0 t1

z" Expect 16" .test
2 t1

z" if/then/else. Expect 42" .test
: t2 if 42 . else 43 . then cr ;
23 t2

z" Expect 43" .test
0 t2

z" Finished" .test
