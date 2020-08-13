%{
#include <iostream>

#include "yacc.tab.h"

using namespace std;
%}

%option noyywrap

delim		[ \t\n\r\v]
ws		{delim}+
var		[[:alpha:]][[:alnum:]]*

ascii_char	[^\"\n]
escaped_char	\\n|\\\"
text		\"({ascii_char}|{escaped_char})*\"

%%

{ws}		{ }
PRINT		{cout << "print "; return tPRINT; }
{var} 		{ cout << "num ";  return 1;}
{var}\$ 	{ cout << "str "; return 1; }
