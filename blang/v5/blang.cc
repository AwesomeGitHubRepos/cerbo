#include <any>
#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <vector>

#include <FlexLexer.h>

//from deque imp
//using std::cout;
//using std::endl;


#include "blang.h"
#include "blang.tab.hh"

using namespace std;


vector<tac> tacs;

int add_tac(int type, int arg1, int arg2)
{
	tacs.push_back(tac{type, arg1, arg2});
	return tacs.size() -1;
}

deque<deque<string>> frames;

void create_frame()
{
	deque<string> frame;
	frames.push_back(frame);
}

void emit_frame()
{
	for(auto& s: frames.front())
		cout << s << "\n";
	frames.pop_front();
}

void trace(std::string text)
{
	cout << "trace:" << text << ".\n" << endl;
}

void emit(std::string text)
{
	//static int opcode_num = 0;
	//printf("%3d\t%s\n", opcode_num, text.c_str());
	//cout << text << "\n";
	//opcode_num++;
	frames.back().push_back(text);
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

std::string str(pnode_t& pnode)
{
	//pnode_t result;
	if(std::holds_alternative<prim_t>(pnode)) {
		prim_t prim = std::get<prim_t>(pnode);
		if(std::holds_alternative<double>(prim))
			return std::to_string(std::get<double>(prim));
		else
			return std::get<std::string>(prim);
	}

	/* 
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
*/

	assert(false);
	//return result;
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

void disassemble()
{
	cout << "Disasembly:\n";
	for(auto& tac: tacs) {
		cout << tac.type << "\t" << tac.arg1 << "\t" << tac.arg2 << "\n";
	}
}

int eval(int pc)
{
	tac t = tacs[pc];
	switch(t.type) {
		case STATEMENT: 
			//trace("Found statement");
			eval(t.arg1);
			if(t.arg2 != -1) eval(t.arg2);
			//if(t.arg2 == -1)
			//	eval(t.arg1);
			//else
			//	eval(t.arg2);
			break;
		case JUST:
			//trace("Found just");
			cout << "just " << eval(t.arg1) << "\n";
			break;
		case PLUS:
			return eval(t.arg1) + eval(t.arg2);
		case INTEGER:
			return t.arg1;
		default:
			trace("Unhandled type");
	}
	return 0;
}

int main()
{
	extern int yyparse();

	//create_frame();
	auto ret = yyparse();
	//emit_frame();
	assert(ret == 0);
	//cout << top << "\n";

	eval(top);

	//disassemble();
	//eval(top_prog_node);

	return 0;
}
