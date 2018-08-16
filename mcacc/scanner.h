#pragma once

extern int yylex();
extern char* yytext;

enum token { NEWLINE = 1, TEXT };

