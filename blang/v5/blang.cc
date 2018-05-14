#include <iostream>

#include <FlexLexer.h>

using std::cout;

#include "blang.h"


//class FlexLexer;

int main()
{
	//yylex();
	//FlexLexer* lexer = new yyFlexLexer;
	yyFlexLexer lexer; // = new yyFlexLexer;
	cout << lexer.yylex() << "\n";
	cout << lexer.yylex() << "\n";
	return 0;
}
