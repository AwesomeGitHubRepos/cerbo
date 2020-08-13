#include <iostream>
using namespace std;

extern FILE* yyin;
extern char* yytext;
extern int yylex();

void yyerror(const char*)
{
	cerr << "ohoh spagetyo\n";
}

int main(int argc, char *argv[])
{
	yyin = fopen(argv[1], "r");

	while(yylex()) {
		cout << "yytext = " << yytext << "\n";
	}

	fclose(yyin);
	return 0;
}
