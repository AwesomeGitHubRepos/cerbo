%{
#include <iostream>
#include "blang.h"
#include "yacc.tab.h"

using namespace std;

%}

%option noyywrap

delim		[ \t\n\r\v]
ws		{delim}+
var		[[:alpha:]][[:alnum:]]*
num		[[:digit:]]+

ascii_char	[^\"\n]
escaped_char	\\n|\\\"
text		\"({ascii_char}|{escaped_char})*\"

%%

{ws}		{ }
PRINT          	{ xnote("print"); return tPRINT; }
{text}		{ return tTEXT; }
{var}          	{ xnote("num");  return 1;}
{var}\$      	{ xnote("str"); return 1; }
{num}		{ return tNUM; }
.		{ return (int) yytext[0]; }
