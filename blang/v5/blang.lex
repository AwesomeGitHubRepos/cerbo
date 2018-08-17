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

%}

ws	[ \t\r\n]
%%

[\t\r ] // discard whitespace
\n	line_number++;
\"[^"]*\"	{ yylval = std::string(yytext); return TEXT; }
"+"		{ return PLUS;}
[0-9]+		{ yylval = std::stod(yytext); 	return INTEGER;}
just{ws}	{ return JUST;}
[a-zA-Z]+ 	{ yylval = std::string(yytext); return IDENT; }

%%
