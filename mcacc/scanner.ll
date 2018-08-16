%{
#include "scanner.h"
#include "parser.hh"
%}

%option noyywrap


newline \n
comment "#"[^\n]*
whitespace [ \t]+
qstring	\"[^\"\n]*\"
text	[^ \t\n\"]+


%%
{comment}	{}
{newline}	{ return L_NEWLINE; }
{whitespace}	{}
{qstring}	{ yylval = yytext; return L_TEXT; }
{text}		{ yylval = yytext; return L_TEXT; }

%%

