0 prompt
: expect  cr "Expect " type type  ":" type cr ;

"hello world" expect
"hi" find execute


"33" expect
variable foo
33 foo !
"foo" find cell + @ .

cr
