%{
#include "blang.h"
//void note(char* m) {}
%}

%token tNUM
%token tPRINT

%%

program	: print_stm
	;

print_stm 	: tPRINT expr { note("got PRINT"); }
	;

expr	: tNUM { $$ = mkint(yytext); }
     	| expr '+' expr { note("got TAC_ADD"); $$ = mkbin(TAC_ADD, $1, $3);  }
     	;

%%

