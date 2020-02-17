0 prompt
\ lets play with polymorphism

\ : @f* @ swap @ f* ;

: cell+ cell + ;
: t! cell+ ! ;
: t@ cell+ @ ;

: tf* swap t@ f* ;
create t:flt ' f. , ' tf* ,

: t* swap t@ * ;
create t:int ' .  , ' t* ,


: terr "Operation not supported" type cr ;
create t:str ' type , ' terr ,

: t. dup t@ swap @ 0 cells+ @ execute ;
: t* dup t@ swap @ 1 cells+ @ execute ;

create f1 t:flt , 0 ,
create f2 t:flt , 0 ,

\ variable f1
12.3 f1 t!
11.0 f2 t!
f1 f2 t*
f1 t!
f1 t. cr

create i1 t:int , 0 ,
create i2 t:int , 0 ,

12 i1 t!
11 i2 t!
i1 i2 t*
i1 t!
i1 t. cr


create s1 t:str , 0 ,
"hello world" s1 t!
s1 t. cr
.s
cr
