\ To run: gforth metta.4th
create yytext 80 allot
variable yylen
\ create yymtext 80 allot
\ variable yymlen
\ : .> type ." >" cr ;
\ : .m yymtext yymlen @  type ;
: .yytext	yytext yylen @  type ; \ print yytext
: yylex parse-name yylen !  yytext yylen @  move ; \ scan next token
\ : Y->M	yytext yymtext yylen @ move yylen @ yymlen !   yylex ; 
: yy0 yytext c@ ;
\ : one 1 ;
\ : // 	POSTPONE DUP POSTPONE  ?exit POSTPONE DROP   ; immediate 
\ : =>	POSTPONE DUP POSTPONE 0= POSTPONE ?exit POSTPONE y->m ; immediate
\ : .NUMBER yy0 digit? swap drop  => ." LDA " .m  cr ;
: IS?	yy0 = ; \ ( c -- t ) is first char of lexeme c?
: NUM?	yy0 '0 >= yy0 '9 <= and ;

\ NOW WE DEFINE OUR ACTUAL RULES

DEFER ex1

: BRA yylex ex1 yylex ;
: EX3	num? if ." LDA " .yytext cr yylex else BRA then ;
: EX2	EX3  begin '* is? while yylex EX3   ." MUL" cr  repeat  ;
: IS-EX1	EX2  begin '+ is? while yylex EX2   ." ADD" cr  repeat ;
' IS-EX1 IS EX1

: go1  refill drop ;
: go go1 yylex    ex1 cr .s  bye ;
go 
1 + ( 5 + ( 2  +  3 * 4 ) * 6 )
21 + 22 * 23 + 26 * ( 24 + 25  ) 
