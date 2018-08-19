%{
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

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
%token LRB RRB PLUS MUL DIV POW 
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
  if_statement   
| JUST expr SEMI { $$ = add_tac(JUST, $2, 0); }
;

expr:
  expr PLUS expr { $$ = add_tac(PLUS, $1, $3);  }
| expr SUB expr { $$ = add_tac(SUB, $1, $3);  }
| expr MUL expr { $$ = add_tac(MUL, $1, $3); }
| expr DIV expr { $$ = add_tac(DIV, $1, $3); }
| expr POW expr	{ $$ = add_tac(POW, $1, $3); }
| SUB expr %prec UMINUS { $$ = add_tac(UMINUS, $2, 0); } 
| LRB expr RRB	{ $$ = $2;}
| INTEGER { $$ = add_tac(INTEGER, $1, 0);  }
;

if_statement:
  IF expr THEN statements FI { $$ = add_tac(IF, $2, $4);  } 
| IF expr THEN statements ELSE statements FI { $$ = add_tac(ELSE, $2, $4, $6); }
;  


///////////////////////////////////////////////////////////////////////
%%
