\ To run: gforth metta.4th
create yytext 80 allot
variable yylen
: .yytext	yytext yylen @  type ; \ print yytext
: yylex parse-name yylen !  yytext yylen @  move ; \ scan next token
: yy0 yytext c@ ;
: IS?	yy0 = ; \ ( c -- t ) is first char of lexeme c?
: NUM?	yy0 '0 >= yy0 '9 <= and ;

\ NOW WE DEFINE OUR ACTUAL RULES

defer ex1

: BRA 	yylex ex1 yylex ;
: EX3	num? if ." LDA " .yytext cr yylex else BRA then ;
: EX2	EX3  begin '* is? while yylex EX3   ." MUL" cr  repeat  ;
: EX1'	EX2  begin '+ is? while yylex EX2   ." ADD" cr  repeat ;

' ex1' IS ex1

: RUN 	refill drop  yylex    ex1 cr .s  bye ;
run
1 + ( 5 + ( 2  +  3 * 4 ) * 6 )
21 + 22 * 23 + 26 * ( 24 + 25  ) 
