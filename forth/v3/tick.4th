0 prompt
: .sn .s cr ;
\ : undefined "Undefined word:<" type type ">" type cr ; \ ( token -- )
\ : (undefined) "ERR: undefined word" type cr ;

\ : (')	parse-word find ;
\ : ' parse-word dup find dup if swap drop else  drop undefined  [ (')  .undefined ] literal then ;
\ : ' parse-word dup find dup if swap drop else  drop undefined  [ parse-word .undefined find ] literal then ;
\ see '

\ : foo [ ' .undefined ] literal ; .s

\ foo execute
\ see foo

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

