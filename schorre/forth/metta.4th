create yytext 80 allot
variable yylen
create yymtext 80 allot
variable yymlen
: .> type ." >" cr ;
: .m yymtext yymlen @  type ;
: .yytext	yytext yylen @  type ; \ print yytext
: yylex parse-name yylen !  yytext yylen @  move ; \ scan next token
: Y->M	yytext yymtext yylen @ move yylen @ yymlen !   yylex ; 
: yy0 yytext c@ ;
: one 1 ;
: // 	POSTPONE DUP POSTPONE  ?exit POSTPONE DROP   ; immediate 
: =>	POSTPONE DUP POSTPONE 0= POSTPONE ?exit POSTPONE y->m ; immediate
: .NUMBER yy0 digit? swap drop  => ." LDA " .m  cr ;
: IS?	yy0 = ; \ ( c -- t ) is first char of lexeme c?
\ : $<	here ; immediateI

\ NOW WE DEFINE OUR ACTUAL RULES
: BRA  '( is?  => ." bracket" cr ; 

: xex1 ;
\ : EX1 	 .NUMBER  // BRA ;
: EX3	 .NUMBER  drop ;
: EX2	EX3  begin '* is? yylex while EX3   ." MUL" cr  repeat .s ;
: EX1	EX2  begin '+ is? yylex while EX2   ." ADD" cr  repeat ;

: xwhat ;
: what ." source <" source .>  ." what stack:"  ;
: go1  refill drop xwhat ;
: go go1 yylex    ex1 cr .s  bye ;

: go2 0 => ." hello" ;
: go2a 0 => ." world" ;
: go3a go2 // go2a ;
: go3 go3a .s bye ;
go 
21 + 22 + 23 + 24 + 25 ( 2 ( 23 ( 22 ( 1 + 2 ) * 
hello world
two lines
