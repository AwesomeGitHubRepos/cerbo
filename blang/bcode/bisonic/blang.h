#pragma once

extern int yylex();
extern void yyerror(const char *);
extern int yyparse();
extern char* yytext;

void note(const char* msg);
void xnote(const char* msg);
