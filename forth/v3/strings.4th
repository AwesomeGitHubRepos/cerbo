0 prompt
\ Test out strings
." Expect: <hello world>" cr
"<hello world>" type cr cr


." Expect: <hello again>" cr
: s "<hello " "again>" swap type type cr ;
s

