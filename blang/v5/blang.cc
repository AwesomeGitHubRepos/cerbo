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

pnode_t make_funcall(pnode_t& identifier, int TODO)
{
	/*
	pnode_t fn = [=]() {
		func_t func = funcmap[enstr(identifier)];
		pnode_t p = func;
		return p;
	};
	*/
	funcall_c fc;
	fc.function_name = enstr(identifier);
	//return funcmap[enstr(identifier)];
	return fc;
}

prim_t eval(pnode_t& pnode)
{
	//pnode_t result;
	if(std::holds_alternative<prim_t>(pnode)) {
		prim_t prim = std::get<prim_t>(pnode);
		if(std::holds_alternative<double>(prim))
			return std::get<double>(prim);
		else
			return std::get<std::string>(prim);
	} else if(std::holds_alternative<funcall_c>(pnode)) {
		funcall_c& fc = std::get<funcall_c>(pnode);
		func_t& fn = funcmap[fc.function_name];
		prims_t vals;
		std::transform(fc.pnodes.begin(), fc.pnodes.end(),
				std::back_inserter(vals),				
				eval); 
		return fn(vals);
	}

	assert(false);
	//return result;
}

int main()
{
	extern int yyparse();

	auto ret = yyparse();
	assert(ret == 0);

	eval(top_prog_node);

	return 0;
}
