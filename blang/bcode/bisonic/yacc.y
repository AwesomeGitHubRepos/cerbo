%{
#include "blang.h"
//void note(char* m) {}
%}

%token tNUM
%token tPRINT

%left '+'

%%

program	: stm_list
	;

stm_list	: stm
		| stm_list stm { $$ = join_tac($1, $2); }
		;
		
stm		: print_stm
     		;

print_stm 	: tPRINT expr { $$ = mkstm(TAC_PRINT, $2); }
		;

expr		: tNUM { $$ = mkint(yytext); }
     		| expr '+' expr { $$ = mkbin(TAC_ADD, $1, $3);  }
     		;

%%

