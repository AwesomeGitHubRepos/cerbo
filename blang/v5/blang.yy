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
%token JUST
%token LRB RRB PLUS MUL
%token SEMI

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
  JUST expr SEMI { trace("just"); pnodes_c p ; p.append($1); $$ = p; }
;

expr:
  expr PLUS expr { emit("+") ; $$ = make_funcall("+", $1, $3); }
| expr '-' expr { $$ = make_funcall("-", $1, $3); }
| expr MUL expr { emit("*"); $$ = make_funcall("*", $1, $3); }
| expr '/' expr { $$ = make_funcall("/", $1, $3); }
| expr '^' expr	{ $$ = make_funcall("^", $1, $3); }
| '-' expr %prec UMINUS { $$ = make_funcall("-", 0, $2); } 
| LRB expr RRB	{ $$ = $2;}
| INTEGER { emit(str($1)) ; trace("INTEGER"); }
;




///////////////////////////////////////////////////////////////////////
%%
