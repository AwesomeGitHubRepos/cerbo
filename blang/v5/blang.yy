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

YYSTYPE join(byte_t b, YYSTYPE vec1, YYSTYPE vec2)
{
	YYSTYPE vec{b};
	vec = join(vec, join(vec1, vec2));
	//vec1.insert(vec1.end(), vec2.begin(), vec2.end());
	return vec;
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

statements	:	expression
			{ $$ = $1; }
		| 	statements expression
			{ $$ = join($1, $2); }

expression	:	expression PLUS expression
	   		{ $$ = join((byte_t) (PLUS-HALT), $1, $3); }
		|	INTEGER
			{ $$ = $1; }

///////////////////////////////////////////////////////////////////////
%%
