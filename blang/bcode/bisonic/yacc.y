%{
extern int yylex();
extern void yyerror(const char *);
%}

%token tPRINT

%%

program	: tPRINT
	;

