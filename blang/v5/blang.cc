#include <cassert>
#include <cmath>
#include <iostream>
#include <stack>
#include <vector>

#include <FlexLexer.h>

#include "blang.h"
#include "blang.tab.hh"

using namespace std;


yyFlexLexer lexer; 
//YYSTYPE top = 0; // root of the program
int top = 0; // root of the program
//vector<tac> tacs;


stack<int> stk;


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


byte_t bget(int& ip)
{
	return bcode[ip++];
}

int iget(int& ip)
{
	int i = 0;
	for(int j = 0; j< sizeof(int); ++j) 
		i += bget(ip) << (8*j);
	//int i = (int) bcode[ip];
	//ip += sizeof(int);
	return i;
}


int pop()
{
	int i = stk.top();
	stk.pop();
	return i;
}

int eval1()
{
	static int ip = 0;       	
	int opcode = bget(ip) + HALT;
	if(opcode==HALT) return 0;

	switch(opcode) {
		case INTEGER:
			stk.push(iget(ip));
			//cout << "integer:" << (int) iget(ip) << "\n";
			break;
		case PLUS:
			eval1();
			eval1();
			stk.push(pop() + pop());
			break;
		default:
			cout << "unknown bcode:" << (int)opcode <<  "\n";				
	}
	return 1;
}

void eval()
{
	while(eval1());

	cout << "Stack is: ";
	while(!stk.empty()) cout << pop() << " ";
	cout << "\n";

	return;

}

int main()
{
	//extern int yyparse();

	//create_frame();
	auto ret = yyparse();
	//emit_frame();
	assert(ret == 0);
	bcode.push_back(0); // HALT
	//cout << top << "\n";

	eval();

	//disassemble();
	//eval(top_prog_node);

	return 0;
}
