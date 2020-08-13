#include <iostream>
using namespace std;

extern char* yytext;
extern int yylex();


int main()
{
	while(yylex()) {
		cout << "yytext = " << yytext << "\n";
	}
	return 0;
}
