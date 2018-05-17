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
\+	return(PLUS);
[a-zA-Z]+ { 
	//yylval.STRING = "foos"s; 
	yylval = strvec.size();
	strvec.push_back(yytext); return(IDENT); }

%%
