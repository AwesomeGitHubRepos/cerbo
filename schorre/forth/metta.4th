create yytext 80 allot
variable yylen
create yymtext 80 allot
variable yymlen
: .> type ." >" cr ;
: $m yymtext yymlen @  type ;
: .yytext	yytext yylen @  type ; \ print yytext
: yylex parse-name yylen !  yytext yylen @  move ; \ scan next token
: save-match yytext yymtext yylen @ move yylen @ yymlen !  ; 
: ~MATCH	if save-match yylex 1 else 0 then ;
: yy0 yytext c@ ;
\ : // compile, IF compile true  compile exit compile THEN ; immediate
: one 1 ;
: // POSTPONE DUP POSTPONE  ?exit POSTPONE DROP   ; immediate 
\ : => POSTPONE ~match POSTPONE DUP POSTPONE 0= POSTPONE ?exit ; immediate
: => POSTPONE DUP POSTPONE 0= POSTPONE ?exit ; immediate
\ : .NUMBER yy0 digit? => ." LDA " $m cr ;
: .NUMBER yy0 digit? => ." LDA " .yytext cr ;
: (? yy0  40 =   ;
: BRA  (?  .s => ." found bracket" ; 

: xex1 ;
: EX1 	 .NUMBER  // BRA ;

: xwhat ;
: what ." source <" source .>  ." what stack:"  ;
: go1  refill drop xwhat ;
: go go1 yylex    ex1  bye ;

: go2 0 => ." hello" ;
: go2a 0 => ." world" ;
: go3a go2 // go2a ;
: go3 go3a .s bye ;
go 
 ( 23 ( 22 ( 1 + 2 ) * 
hello world
two lines
