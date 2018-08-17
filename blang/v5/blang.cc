#include <any>
#include <cassert>
#include <functional>
#include <iostream>

#include <FlexLexer.h>

using std::cout;
using std::endl;


#include "blang.h"


void trace(std::string text)
{
	cout << "trace:" << text << ".\n" << endl;
}


void yyerror(const char* s) {
	cout << "Parse error:" <<  s << endl;
	exit(1);
}

yyFlexLexer lexer; // = new yyFlexLexer;

int yylex()
{
	return lexer.yylex();
}

void pnodes_c::append(const pnode_t& pnode)
{
	pnodes.push_back(pnode);
}

pnodes_c append_expr(pnode_t& vec, const pnode_t& expr)
{
	pnodes_c p = std::get<pnodes_c>(vec); 
	p.append(expr); 
	return  p;
}

pnode_t make_funcall(const std::string& function_name, const pnode_t& pnode1, const pnode_t& pnode2)
{
	funcall_c fc;
	fc.function_name = function_name;
	fc.pnodes.append(pnode1);
	fc.pnodes.append(pnode2);
	return fc;
}

pnode_t make_funcall(pnode_t& identifier, pnode_t& pnodes)
{
	funcall_c fc;
	fc.function_name = enstr(identifier);
	fc.pnodes = std::get<pnodes_c>(pnodes);
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
		std::transform(fc.pnodes.pnodes.begin(), fc.pnodes.pnodes.end(),
				std::back_inserter(vals),				
				eval); 
		return fn(vals);
	} else if(std::holds_alternative<pnodes_c>(pnode)) { 
		prim_t result;
		for(auto& pn: std::get<pnodes_c>(pnode).pnodes)
			result = eval(pn);
		return result;
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
