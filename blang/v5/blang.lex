%option noyywrap

%{
#include "blang.h"

int line_number = 0;

%}

%%

"let"	{ return(LET); }
[\t\r ]* // discard whitespace
\n	line_number++;
[a-zA-Z]+ { return(IDENTIFIER); }

%%
