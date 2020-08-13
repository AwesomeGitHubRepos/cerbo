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

expr	: tNUM { note("NUM:"); note(yytext); }
     	| expr '+' expr { note("+");  }
     	;

%%

