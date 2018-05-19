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

%left '-' '+'
%left '*' '/'
%precedence NEG
%right '^'

///////////////////////////////////////////////////////////////////////
%%

prog: proga { top_prog_node = $1; }


proga:
 expr
;

funcall:
//  IDENT '('  ')' { pnodes_c pn; $$ = make_funcall($1, pn); }
IDENT '(' arglist ')' { $$ = make_funcall($1, $3); }
;

expr:
  PRIM 
| funcall
| expr '+' expr { $$ = make_funcall("+", $1, $3); }
| expr '-' expr { $$ = make_funcall("-", $1, $3); }
| expr '*' expr { $$ = make_funcall("*", $1, $3); }
| expr '/' expr { $$ = make_funcall("/", $1, $3); }
| '-' expr %prec NEG { $$ = make_funcall("-", 0, $2); }
| expr '^' expr	{ $$ = make_funcall("^", $1, $3); }
| '(' expr ')'	{ $$ = $2;}
;


arglist:
  %empty { $$ = pnodes_c(); }       
| expr { pnodes_c p ; p.append($1); $$ = p; }
| arglist ',' expr { pnodes_c p = std::get<pnodes_c>($1); p.append($3); $$ = p ; } 
;

///////////////////////////////////////////////////////////////////////
%%
