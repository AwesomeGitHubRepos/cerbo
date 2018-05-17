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
,	return ',';
\(	return '(';
\)	return ')';
[0-9]+		{ 
	yylval = parsevec.size();
	parsevec.push_back(std::stod(yytext)); 
	return NUM;
}
[a-zA-Z]+ { 
	//yylval.STRING = "foos"s; 
	yylval = parsevec.size();
	parsevec.push_back(std::string(yytext)); return(IDENT); }

%%
