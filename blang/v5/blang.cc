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

YYSTYPE to_bvec(int i)
{
	YYSTYPE res;
	byte_t* arr = (byte_t*) &i;
	for(int j=0; j< sizeof(int); ++j) res.push_back(*(arr+j));
	return res;
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

void do_arith(yytokentype type)
{
	eval1(); 
	int a = pop();
	eval1(); 
	int b = pop();
	int res;
	switch(type) {
		case PLUS:
			res = a+b;
			break;
		case MUL:
			res = a*b;
			break;
		default:
			assert(false);
	}
	push(res);
}

void eval_if()
{
	eval1();
	int cond = pop();
	int jump  = iget();
	if(!cond) ip += jump;
}
void eval_let()
{
	int idx = iget();
	eval1();
	//int value   = pop();
	vars[idx].value = pop();

}

vector<opcode_t> opcodes{
	opcode_t{IF, eval_if},
	opcode_t{INTEGER, [](){ push(iget());} },
	opcode_t{LET, eval_let },
	opcode_t{MUL, [](){ do_arith(MUL);}},
	opcode_t{PLUS, [](){ do_arith(PLUS);}},
	opcode_t{PRINT, [](){ eval1(); cout << pop() << " ";}},
	opcode_t{VAR, [](){ push(vars[iget()].value); }}
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
