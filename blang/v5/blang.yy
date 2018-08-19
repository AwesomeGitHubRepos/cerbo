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


%token STATEMENT
%token IDENT TEXT INTEGER
%token JUST
%token LRB RRB PLUS MUL
%token SEMI
%token IF THEN ELSE FI

%left  SUB PLUS
%left  MUL DIV
%left  POW
%right UMINUS 

///////////////////////////////////////////////////////////////////////
%%

program: statements  { top = $$;}
;

statements:
  statement	       { $$ = add_tac(STATEMENT, $1, -1); }
| statements statement { $$ = add_tac(STATEMENT, $1, $2); }
;

statement:
  if_statement   { $$ = add_tac(IF, $1, 0); }
| JUST expr SEMI { $$ = add_tac(JUST, $2, 0); }
;

expr:
  expr PLUS expr { $$ = add_tac(PLUS, $1, $3);  }
| expr SUB expr {  }
| expr MUL expr { $$ = add_tac(MUL, $1, $3); }
| expr DIV expr {  }
| expr POW expr	{  }
| SUB expr %prec UMINUS {  } 
| LRB expr RRB	{ $$ = $2;}
| INTEGER { $$ = add_tac(INTEGER, $1, 0);  }
;

if_statement:
  IF expr THEN statements FI { $$ = add_tac(IF, $2, $3);  } 
| IF expr THEN statements ELSE statements FI
;  


///////////////////////////////////////////////////////////////////////
%%
