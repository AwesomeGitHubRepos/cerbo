%{
#include "scanner.h"
%}

%option noyywrap


newline \n
comment "#"[^\n]*
whitespace [ \t]+
qstring	\"[^\"\n]*\"
text	[^ \t\n\"]+


%%
{comment}	{}
{newline}	{ return NEWLINE; }
{whitespace}	{}
{qstring}	{ return TEXT; }
{text}		{ return TEXT; }
.		{puts(":("); return 42; }

%%

