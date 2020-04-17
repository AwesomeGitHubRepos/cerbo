0 prompt
\ https://forth-standard.org/standard/core/ColonNONAME
: expect  cr "Expect " type type  ":" type cr ;
"123" expect
:noname 123 . cr ; execute

defer print
:noname . cr ; is print

"23" expect
23 print


variable nn1
variable nn2
:noname 1234 . cr ; nn1 !
:noname 9876 . cr ; nn2 !
"1234" expect
nn1 @ execute
"9876" expect
nn2 @ execute
