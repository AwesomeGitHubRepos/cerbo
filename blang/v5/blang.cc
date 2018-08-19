#include <any>
#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <cmath>
#include <vector>

#include <FlexLexer.h>

//from deque imp
//using std::cout;
//using std::endl;


#include "blang.h"
#include "blang.tab.hh"

using namespace std;


vector<tac> tacs;

int add_tac(int type, int arg1, int arg2, int arg3)
{
	tacs.push_back(tac{type, arg1, arg2, arg3});
	return tacs.size() -1;

}

int add_tac(int type, int arg1, int arg2)
{
	return add_tac(type, arg1, arg2, -1);
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
		case MUL:
			return eval(t.arg1) * eval(t.arg2);
		case PLUS:
			return eval(t.arg1) + eval(t.arg2);
		case POW:
			return pow(eval(t.arg1), eval(t.arg2));
		case INTEGER:
			return t.arg1;
		case IF:
			if(eval(t.arg1)) eval(t.arg2);
			break;			
		case ELSE:
			if(eval(t.arg1)) 
				eval(t.arg2);
			else
				eval(t.arg3);
			break;			

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
