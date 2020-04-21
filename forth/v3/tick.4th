0 prompt

"hello world" expect
' hi execute

"Undefined word:<ho>" expect
' ho

"ERR: undefined word" expect
execute



"1 2 3" expect
: foo 3 2 1 . . . cr ;
' foo execute

"empty stack" expect
.s cr

