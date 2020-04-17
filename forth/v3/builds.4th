0 prompt
: expect  cr "Expect " type type  ":" type cr ;
z" BUILDS.4th ..." type cr
\ testing the dreaded <builds/does via constant
: const <builds , does> @ ;
11 const eleven
12 const twelve

"11" expect
eleven . cr

"12" expect
twelve . cr

"11" expect
eleven . cr

"12" expect

twelve . cr
z" ... finished" type cr
1 prompt
