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

%%

[\t\r ]* // discard whitespace
\n	line_number++;
\+	return '+';
-	return '-';
,	return ',';
\(	return '(';
\)	return ')';
\"[^"]*\"	{ yylval = std::string(yytext); return PRIM; }
[0-9]+		{ yylval = std::stod(yytext); 	return PRIM;}
[a-zA-Z]+ 	{ yylval = std::string(yytext); return IDENT; }

%%
