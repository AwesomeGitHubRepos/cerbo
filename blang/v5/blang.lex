%option noyywrap

%{

#include <iostream>
#include <memory>
#include <string>


using std::cin;
using std::cerr;
using std::cout;

#include "blang.h"
#include "blang.tab.hh"

int line_number = 0;
//extern yylval;

// TODO reinstate these pattersn
// \"[^"]*\"	{ yylval = store_string(yytext); return TEXT; }
// [a-zA-Z]+ 	{ yylval = store_string(yytext); return IDENT; }
%}

ws	[ \t\r\n]
%%

[\t\r ] // discard whitespace
\n	line_number++;
"("		{ return LRB; }
")"		{ return RRB; }
"-"		{ return SUB;}
"+"		{ return PLUS;}
"*"		{ return MUL;}
"/"		{ return DIV; }
"^"		{ return POW; }
";"		{ return SEMI;}
[0-9]+		{ yylval = std::stoi(yytext); 	return INTEGER;}
IF		{ return IF; }
THEN		{ return THEN; }
ELSE		{ return ELSE; }
FI		{ return FI; }
JUST		{ return JUST;}

%%
