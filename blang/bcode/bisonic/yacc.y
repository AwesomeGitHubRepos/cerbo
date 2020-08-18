%{
#include "blang.h"
//void note(char* m) {}
%}

%token tNUM
%token tPRINT
%token tTEXT

%left '+' '-'

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

expr		: num_expr | str_expr;

str_expr	: tTEXT { $$ = mkstr(yytext); }
	 	| str_expr '+' str_expr { $$ = mkbin(TAC_CAT, $1, $3); }

num_expr	: tNUM { $$ = mkint(yytext); }
     		| num_expr '+' num_expr { $$ = mkbin(TAC_ADD, $1, $3);  }
     		| num_expr '-' num_expr { $$ = mkbin(TAC_SUB, $1, $3);  }
     		;

%%

