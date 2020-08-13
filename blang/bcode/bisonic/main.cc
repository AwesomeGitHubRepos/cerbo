#include <iostream>

#include "blang.h"

using namespace std;

extern FILE* yyin;
extern char* yytext;
//extern int yylex();

void yyerror(const char*)
{
	cerr << "ohoh spagetyo\n";
}

void note(const char* msg)
{
	cout << msg << "\n";
}

void xnote(const char* msg) {}

int main(int argc, char *argv[])
{
	yyin = fopen(argv[1], "r");

	/*
	while(yylex()) {
		cout << "yytext = " << yytext << "\n";
	}
	*/

	yyparse();

	fclose(yyin);
	return 0;
}
