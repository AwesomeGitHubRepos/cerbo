%{
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <variant>

#include "blang.h"

using std::cout;
using std::endl;

//extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);
%}



%token IDENT NUM STRING


///////////////////////////////////////////////////////////////////////
%%

prog:
	concat
	| funcall
	| NUM { $$ = make_num($1); };
	;


concat: IDENT '+' IDENT{ $$ = make_concat($1, $3); };

funcall:
	IDENT args { cout << "making funcall\n"; $$ = make_funcall($1, $2) ; } 
	;

args: 
	'(' ')'  
	| '(' arglist ')'
    	;
     
arglist:
      	NUM
	|  arglist ',' NUM
	;


///////////////////////////////////////////////////////////////////////
%%
