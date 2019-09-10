#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include <FlexLexer.h>

#include "blang.h"
#include "blang.tab.hh"

using namespace std;


yyFlexLexer lexer; 
//YYSTYPE top = 0; // root of the program
int top = 0; // root of the program
//vector<tac> tacs;



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

void eval()
{
	int ip = 0;
	while(1) {
		int opcode = bget(ip) + HALT;

		switch(opcode) {
			case HALT:
				goto finis;

			case INTEGER:
				cout << "integer:" << (int) iget(ip) << "\n";
				break;
			default:
				cout << "unknown bcode:" << (int)opcode <<  "\n";				
		}
	}
finis:
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
