#include <any>
#include <cassert>
#include <functional>
#include <iostream>

#include <FlexLexer.h>

using std::cout;
using std::endl;


#include "blang.h"




void yyerror(const char* s) {
	cout << "Parse error:" <<  s << endl;
	exit(1);
}

yyFlexLexer lexer; // = new yyFlexLexer;

int yylex()
{
	return lexer.yylex();
}

pnode_t eval(pnode_t& pnode)
{
	pnode_t result;
	if(std::holds_alternative<prim_t>(pnode)) {
		prim_t prim = std::get<prim_t>(pnode);
		if(std::holds_alternative<double>(prim))
			return std::get<double>(prim);
		else
			return std::get<std::string>(prim);
	}

	assert(false);
	return result;
}

int main()
{
	extern int yyparse();

	auto ret = yyparse();
	assert(ret == 0);

	eval(top_prog_node);

	return 0;
}
