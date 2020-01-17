0 prompt
z" BUILDS.4th ..." type cr
\ testing the dreaded <builds/does via constant
: const <builds , does> @ ;
11 const eleven
12 const twelve
z" Expect 11:" type cr
eleven . cr
z" Expect 12" type cr
twelve . cr
z" Expect 11:" type cr
eleven . cr
z" Expect 12" type cr
twelve . cr
z" ... finished" type cr
1 prompt
