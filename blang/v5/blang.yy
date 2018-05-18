%{
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <variant>

#include "blang.h"

using std::cout;
using std::endl;

extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);
%}



%token IDENT NUM STRING


///////////////////////////////////////////////////////////////////////
%%

prog: proga { top_prog_node = $1; }


proga:
     	IDENT '(' ')' { $$ = make_funcall($1, 666); }
	| NUM { $$ = $1 ; }
	;


///////////////////////////////////////////////////////////////////////
%%
