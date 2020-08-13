%{
#include <iostream>
using namespace std;
%}

%option noyywrap

delim	[ \t\n\r\v]
ws	{delim}+
var	[[:alpha:]][[:alnum:]]*
%%

{ws}	{ }
PRINT	{cout << "print "; return 1; }
{var} { cout << "num ";  return 1;}
{var}\$ { cout << "str "; return 1; }
