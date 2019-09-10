#include <cassert>
#include <cmath>
#include <functional>
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


static int ip = 0;       	
stack<int> stk;

int eval1();

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
int iget() { return iget(ip); }


int pop()
{
	int i = stk.top();
	stk.pop();
	return i;
}

void push(int i) { stk.push(i); }

typedef std::function<void()> func_t;
typedef struct { yytokentype opcode; func_t fn; } opcode_t;

//void do_int() { push(666); }

vector<opcode_t> opcodes{
	//opcode_t{INTEGER, do_int },
	opcode_t{INTEGER, [](){ push(iget());} },
	opcode_t{PLUS, [](){ eval1(); eval1(); push(pop() + pop());} }
};

map<yytokentype,opcode_t> opmap;

int eval1()
{
	int opcode = bget(ip) + HALT;
	if(opcode==HALT) return 0;

	opcode_t& op = opmap[(yytokentype) opcode];
	op.fn();
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
	for(auto& op:opcodes) opmap[op.opcode] = op;

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
