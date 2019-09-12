%{
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "blang.h"
#include "blang.tab.hh"

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

YYSTYPE join(YYSTYPE vec1, YYSTYPE vec2, YYSTYPE vec3)
{
	return join(vec1, join(vec2, vec3));
}

YYSTYPE join(byte_t b, YYSTYPE vec1, YYSTYPE vec2)
{
	YYSTYPE vec{b};
	vec = join(vec, join(vec1, vec2));
	//vec1.insert(vec1.end(), vec2.begin(), vec2.end());
	return vec;
}

YYSTYPE join_toke(yytokentype toke, YYSTYPE vec1)
{
	YYSTYPE vec{(byte_t) (toke-HALT)};
	vec = join(vec, vec1);
	return vec;
}

YYSTYPE join_toke(yytokentype toke, YYSTYPE vec1, YYSTYPE vec2)
{
	YYSTYPE vec = join_toke(toke, vec1);
	return join(vec, vec2);
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
%token VAR
%token EQ
%token LET
%token JREL

%left  SUB PLUS
%left  MUL DIV
%left  POW
%right UMINUS 

///////////////////////////////////////////////////////////////////////
%%

program: statements  { bcode = $1; top = 0;}
;

statements	:	statement { $$ = $1; }
		| 	statements statement { $$ = join($1, $2); }


statement	:	print_statement
	  	|	assignment
		|	if_statement

assignment	:	VAR EQ expression {$$ = join_toke(LET, $1, $3); }

if_statement	:	IF expression THEN statements FI 
	     		{ 
				$$ = join_toke(IF, $2);
				$$ = join($$, to_bvec($4.size()), $4);
			}
		| 	IF expression THEN statements ELSE statements FI
			{
				$$ = join_toke(IF, $2);
				YYSTYPE &if_clause = $4;
				YYSTYPE else_clause{$6};
				YYSTYPE jump = join_toke(JREL, to_bvec(else_clause.size()));
				if_clause = join(if_clause, jump);
				$$ = join($$, to_bvec(if_clause.size()));
				$$ = join($$, if_clause, else_clause);
			}

print_statement	:	PRINT expression {$$ = join_toke(PRINT, $2); }

expression	:	expression PLUS expression
	   		{ $$ = join_toke(PLUS, $1, $3); }
		|	expression MUL expression
	   		{ $$ = join_toke(MUL, $1, $3); }
		|	INTEGER
			{ $$ = join_toke(INTEGER, $1); }
		|	VAR
			{ $$ = join_toke(VAR, $1); }

///////////////////////////////////////////////////////////////////////
%%
