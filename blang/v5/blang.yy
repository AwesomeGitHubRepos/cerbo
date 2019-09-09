%{
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "blang.h"

using std::cout;
using std::endl;

//extern YYSTYPE yyparse();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);

YYSTYPE join(YYSTYPE vec1, YYSTYPE vec2)
{
	vec1.insert(vec1.end(), vec2.begin(), vec2.end());
	return vec1;
}

%}


%token HALT // must always be first token because all the other ones are relative to this


%token STATEMENT
%token IDENT TEXT INTEGER
%token JUST
%token LRB RRB PLUS MUL DIV POW 
%token SEMI
%token IF THEN ELSE FI
%token PRINT

%left  SUB PLUS
%left  MUL DIV
%left  POW
%right UMINUS 

///////////////////////////////////////////////////////////////////////
%%

program: statements  { bcode = $1; top = 0;}
;

statements:
	  INTEGER { $$ = $1; }
| statements INTEGER { $$ = join($1, $2); }

///////////////////////////////////////////////////////////////////////
%%
