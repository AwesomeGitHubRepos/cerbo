#include <iostream>

#include <FlexLexer.h>

using std::cout;
using std::endl;


#include "blang.h"
//#include "Parser.h"


//class FlexLexer;


void yyerror(const char* s) {
	cout << "Parse error:" <<  s << endl;
	exit(1);
}

yyFlexLexer lexer; // = new yyFlexLexer;

int yylex()
{
	return lexer.yylex();
}


int main()
{
	extern int yyparse();

	//while(lexer.yylex())
	//	cout << "Token:`" << lexer.YYText() << "'\n";
	yyparse();	

	//Parser parser;
	//Parser.parse();
	return 0;
}
