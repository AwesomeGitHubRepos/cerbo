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

	while(lexer.yylex())
		cout << "Token:`" << lexer.YYText() << "'\n";
	return 0;
}
