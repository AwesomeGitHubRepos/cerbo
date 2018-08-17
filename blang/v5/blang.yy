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



%token IDENT TEXT INTEGER

%left '-' '+'
%left '*' '/'
%left '^'
%right UMINUS 

///////////////////////////////////////////////////////////////////////
%%

program: subitems



subitems:
  subitem	
| subitems subitem
;

subitem:
  "just" expr { pnodes_c p ; p.append($1); $$ = p; }
;

expr:
  expr '+' expr { $$ = make_funcall("+", $1, $3); }
| expr '-' expr { $$ = make_funcall("-", $1, $3); }
| expr '*' expr { $$ = make_funcall("*", $1, $3); }
| expr '/' expr { $$ = make_funcall("/", $1, $3); }
| expr '^' expr	{ $$ = make_funcall("^", $1, $3); }
| '-' expr %prec UMINUS { $$ = make_funcall("-", 0, $2); } 
| '(' expr ')'	{ $$ = $2;}
| INTEGER
;




///////////////////////////////////////////////////////////////////////
%%
