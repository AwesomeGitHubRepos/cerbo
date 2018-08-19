#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include <FlexLexer.h>

#include "blang.h"
#include "blang.tab.hh"

using namespace std;


yyFlexLexer lexer; 
int top = 0; // root of the program
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


void trace(std::string text)
{
	cout << "trace:" << text << ".\n" << endl;
}


void yyerror(const char* s) {
	cout << "Parse error:" <<  s << endl;
	exit(1);
}


int yylex()
{
	return lexer.yylex();
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
		case UMINUS:
			return - eval(t.arg1);
		case DIV:
			return eval(t.arg1) / eval(t.arg2);
		case MUL:
			return eval(t.arg1) * eval(t.arg2);
		case PLUS:
			return eval(t.arg1) + eval(t.arg2);
		case SUB:
			return eval(t.arg1) - eval(t.arg2);
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
