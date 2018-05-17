%{
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <variant>

#include "blang.h"

using std::cout;
using std::endl;

//extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);
%}



%token IDENT NUM PLUS STRING


///////////////////////////////////////////////////////////////////////
%%

prog:
	concat
	| NUM { $$ = make_num($1); };
	;

concat: IDENT PLUS IDENT{ $$ = make_concat($1, $3); };

///////////////////////////////////////////////////////////////////////
%%
