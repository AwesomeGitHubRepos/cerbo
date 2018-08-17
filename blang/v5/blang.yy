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
%left '^'
%right UMINUS

///////////////////////////////////////////////////////////////////////
%%

prog: proga { top_prog_node = $1; }


proga:
  %empty { $$ = pnodes_c(); }       
| expr { pnodes_c p ; p.append($1); $$ = p; }
| proga expr { $$ = append_expr($1, $2); }
;

/*
funcall:
IDENT '(' arglist ')' { $$ = make_funcall($1, $3); }
;
*/

expr:
  expr '+' expr { $$ = make_funcall("+", $1, $3); }
| expr '-' expr { $$ = make_funcall("-", $1, $3); }
| expr '*' expr { $$ = make_funcall("*", $1, $3); }
| expr '/' expr { $$ = make_funcall("/", $1, $3); }
| expr '^' expr	{ $$ = make_funcall("^", $1, $3); }
| '-' expr %prec UMINUS { $$ = make_funcall("-", 0, $2); }
| '(' expr ')'	{ $$ = $2;}
| "PRIM"
| IDENT '(' argument_list ')' { $$ = make_funcall($1, $3); }
;

argument_list:
  %empty { $$ = pnodes_c(); }
| expression_list
;

expression_list:
  expr { pnodes_c p ; p.append($1); $$ = p; }
| expression_list ',' expr { $$ = append_expr($1, $3); }
;

/*
arglist:
  %empty { $$ = pnodes_c(); }       
| expr { pnodes_c p ; p.append($1); $$ = p; }
| arglist ',' expr { $$ = append_expr($1, $3); }
;
*/


///////////////////////////////////////////////////////////////////////
%%
