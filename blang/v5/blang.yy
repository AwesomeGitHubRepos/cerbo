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



%token IDENT PRIM


///////////////////////////////////////////////////////////////////////
%%

prog: proga { top_prog_node = $1; }


proga:
     	funcall
	| PRIM { $$ = $1 ; }
	;

funcall:
     	IDENT '(' arglist ')' { $$ = make_funcall($1, $3); }

arglist:
       %empty { $$ = pnodes_c();}
	| PRIM { pnodes_c p ; p.pnodes.push_back($1); $$ = p; }
	| arglist ',' PRIM { pnodes_c p = std::get<pnodes_c>($1); p.pnodes.push_back($3); $$ = p ; } 

///////////////////////////////////////////////////////////////////////
%%
