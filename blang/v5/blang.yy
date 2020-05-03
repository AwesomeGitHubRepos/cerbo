%{
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include "blang.h"
//#include "blang.tab.hh"

using std::cout;
using std::endl;

//extern YYSTYPE yyparse();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);


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
%token GOTO LABEL
%token KSTR
%token SX // statement terminator. Used for commands with unlimited arguments

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
		|	label_statement
		|	goto_statement
		|	just_statement
		;

just_statement	:	JUST arg { $$ = join_toke(JUST, $2); }
	       	;

assignment	:	VAR EQ expression {$$ = join_toke(LET, $1, $3); }

if_statement	:	IF expression THEN statements FI 
	     		{ 
				$$ = join($2, join_toke(IF, $4.size()), $4);
			}
		| 	IF expression THEN statements ELSE statements FI
			{
				YYSTYPE &else_clause = $6;
				YYSTYPE jump = join_toke(JREL, else_clause.size());
				YYSTYPE if_clause = join($4, jump);
				$$ = join($2, join_toke(IF, if_clause.size()), if_clause, else_clause);
			}

goto_statement	:	GOTO LABEL { $$ = join_toke(GOTO, $2); }
label_statement	:	LABEL { $$ = join_toke(LABEL, $1); } // doesn't check for duplicate labels

kstr		:	KSTR { $$ = join_toke(KSTR, $1); }

print_statement	:	PRINT print_list {$$ = join_toke(PRINT, $2, YYSTYPE{SX-HALT}); }

print_list	:	arg {$$ = $1; }
	   	|	print_list arg {$$ = join($1, $2); }

arg		:	kstr { $$ = $1; } 
     		|	expression { $$ = $1 ; }

expression	:	expression PLUS expression { $$ = join_toke(PLUS, $1, $3); }
		|	expression SUB expression { $$ = join_toke(SUB, $1, $3); }
		|	expression MUL expression { $$ = join_toke(MUL, $1, $3); }
		|	expression DIV expression { $$ = join_toke(DIV, $1, $3); }
		|	SUB expression %prec UMINUS { $$ = join_toke(UMINUS, $2); }
		|	LRB expression RRB { $$ = $2; }
		|	INTEGER { $$ = join_toke(INTEGER, $1); }
		|	VAR { $$ = join_toke(VAR, $1); }

///////////////////////////////////////////////////////////////////////
%%
