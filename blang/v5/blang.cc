#include <cassert>
#include <cmath>
#include <fstream>
//#include <functional>
#include <iostream>
#include <stack>
//#include <vector>

#include <FlexLexer.h>

#include "blang.h"
//#include "blang.tab.hh"

using namespace std;


yyFlexLexer lexer; 
int top = 0; // root of the program


static int ip = 0;       	
//stack<int> stk;
stack<val_t> stk;

typedef std::function<void()> func_t;
typedef struct { yytokentype opcode; string name; int size; func_t fn; } opcode_t;
map<yytokentype,opcode_t> opmap;

string stringify(val_t v)
{
	if(auto pval = get_if<float>(&v))
		return to_string(*pval);

	if(holds_alternative<string>(v))
		return get<string>(v);

	throw 666;
}

bool truthify(val_t v)
{
	if(auto pval = get_if<float>(&v))
		return *pval != 0 ;

	if(auto pval = get_if<string>(&v))
		return pval->size() > 0;

	throw 666;
}

int eval1();

YYSTYPE join(YYSTYPE vec1, YYSTYPE vec2)
{
	vec1.insert(vec1.end(), vec2.begin(), vec2.end());
	return vec1;
}

YYSTYPE join(YYSTYPE vec1, YYSTYPE vec2, YYSTYPE vec3)
{
	return join(vec1, join(vec2, vec3));
}

YYSTYPE join(YYSTYPE vec1, YYSTYPE vec2, YYSTYPE vec3, YYSTYPE vec4)
{
	return join(vec1, join(vec2, vec3, vec4));
}
YYSTYPE join(byte_t b, YYSTYPE vec1, YYSTYPE vec2)
{
	YYSTYPE vec{b};
	vec = join(vec, join(vec1, vec2));
	//vec1.insert(vec1.end(), vec2.begin(), vec2.end());
	return vec;
}

YYSTYPE join_toke(yytokentype toke, YYSTYPE vec1)
{
	// verify that toke is evaluable
	if( opmap.find(toke) == opmap.end()) {
		//cerr << "ERROR: Couldn't find evaluater for opcode " << toke << endl;
		throw std::out_of_range("No evaluator opcode:"s + to_string(toke) + " in file " + __FILE__ + " line " + to_string(__LINE__));
	}


	YYSTYPE vec{(byte_t) (toke-HALT)};
	vec = join(vec, vec1);
	return vec;
}

YYSTYPE join_toke(yytokentype toke, int i)
{
	return join_toke(toke, to_bvec(i));
}

YYSTYPE join_toke(yytokentype toke, YYSTYPE vec1, YYSTYPE vec2)
{
	YYSTYPE vec = join_toke(toke, vec1);
	return join(vec, vec2);
}

int make_kstr (char* chars)
{
	string str = chars;
	int res = kstrs.size();
	kstrs.push_back(str);
	return res;
}

/*
YYSTYPE make_kstr(YYSTYPE bvec)
{
	//puts(yylval);
	//string str = get<string>(v);
	string str;
	str.reserve(yylval.size());
	for(int i = 0; i< bvec.size(); i++) str[i] = bvec[i];
	//str = "TODO"s;
	kstrs.push_back(str);
	cout << "make_kstr:"s << str.size() << ":" << kstrs.size()-1 << ":" << str << endl;
	//return join_toke(KSTR, to_bvec(str.size()), str);
	return join_toke(KSTR, kstrs.size() -1);
}
*/

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

int iget (int& ip)
{
	int i = 0;
	for(int j = 0; j< sizeof(int); ++j) 
		i += bget(ip) << (8*j);
	//int i = (int) bcode[ip];
	//ip += sizeof(int);
	return i;
}
int iget() { return iget(ip); }


val_t pop()
{
	val_t res = stk.top();
	stk.pop();
	return res;
}

//void push(int i) { stk.push(i); }
void push (val_t v) { stk.push(v); }


//void do_int() { push(666); }

void do_arith(yytokentype type)
{
	eval1(); 
	int a = get<float>(pop());
	eval1(); 
	int b = get<float>(pop());
	int res;
	switch(type) {
		case PLUS:
			res = a+b;
			break;
		case SUB:
			res = a-b;
			break;
		case MUL:
			res = a*b;
			break;
		case DIV:
			res = a/b;
			break;
		default:
			assert(false);
	}
	push(res);
}

void eval_uminus()
{
	eval1();
	push(-get<float>(pop()));
}
void eval_goto()
{
	int idx = iget();

	ip = get<float>(labels[idx].value);
}

void eval_if()
{
	//eval1();
	bool cond = truthify(pop());
	int jump  = iget();
	if(!cond) ip += jump;
}

void eval_jrel()
{ 
	int off = iget(); 
	ip+= off;
}

void eval_let()
{
	int idx = iget();
	eval1();
	vars[idx].value = pop();

}


void eval_kstr()
{
	int idx = iget();
	//cout << "IP" << ip << ",String length is: "s << idx <<endl;
	string str = kstrs[idx];
	//cout << "eval_kstr:" << idx << "," << str << endl;
	push(str);
	cout << "eval_kstr:pushed:" << str << endl;
	flush(cout);

}

string strpop() { return stringify(pop()); }

void eval_print()
{
	//int opcode;
	cout << "eval_print:entered" << endl;
	while(1) {
		int opcode = bget(ip) + HALT;
		if(opcode == SX) break;		
		cout << "eval_print: something:" << opcode << endl;
		eval1(); 
		cout << strpop() << " \n"; 
		std::flush(cout);
	}
	cout << "eval_print:exiting" << endl;
}

void eval_just()
{
	eval1();
	cout << "Just:" << strpop() << "\n";
}

const auto int1 = sizeof(int);
const auto int2 = 2*sizeof(int);

vector<opcode_t> opcodes{
	opcode_t{UMINUS, "UMINUS", 0,	eval_uminus},
	opcode_t{JUST,	"JUST",	0,	eval_just},
	opcode_t{SX,	"SX",	0,	[](){;}}, // do nothing, it's a statement terminator
	opcode_t{KSTR,	"KSTR", int1,	eval_kstr},
	opcode_t{GOTO,	"GOTO",	int1, 	eval_goto},
	opcode_t{IF, 	"IF",	int1, 	eval_if},
	opcode_t{INTEGER, "INT",int1, [](){ push(iget());}},
	opcode_t{JREL, 	"JREL",	int1, 	eval_jrel},
	opcode_t{LABEL, "LAB",	int1, 	[](){ iget(); }},
	opcode_t{LET, 	"LET",	int1, 	eval_let },
	opcode_t{MUL, 	"MUL",	0, 	[](){ do_arith(MUL);}},
	opcode_t{DIV, 	"DIV",	0, 	[](){ do_arith(DIV);}},
	opcode_t{PLUS, 	"PLUS",	0, 	[](){ do_arith(PLUS);}},
	opcode_t{PRINT, "PRINT",0, 	eval_print},
	opcode_t{SUB, 	"SUB",	0,	[](){ do_arith(SUB);}},
	opcode_t{VAR, 	"VAR",	int1, [](){ push(vars[iget()].value); }}
};


int eval1 ()
{
	int opcode = bget(ip) + HALT;
	if(opcode==HALT) return 0;

	opcode_t& op = opmap[(yytokentype) opcode];
	try {
		op.fn();
	} catch (std::bad_function_call& e) {
		cout << "ERROR:eval1:Bad function:" << opcode << ":`" << op.name << "'\n";
		throw e;
	}


	return 1;
}

void print_stack ()
{
	if(stk.empty()) return;
	cout << "Stack is: ";
	while(!stk.empty()) cout << stringify(pop()) << " ";
	cout << "\n";
}


void decompile()
{
	//return;
	ofstream ofs;
	ofs.open("decompile.txt");

	ip = 0;
	while(1) {
		auto opcode = bget(ip) + HALT;
		if(opcode==HALT) goto finis;
		opcode_t& op = opmap[(yytokentype) opcode];
		ofs << op.name << " ";
		int ival = iget();
		switch(op.opcode) {
			case INTEGER: //fal;
			case GOTO: // fallthrough
			case LABEL:
				ofs << ival;
				break;
			//case SX:
			//	cout << 
			case KSTR:
				cout << ival << "\t" << kstrs[ival];
				goto inc;
			default:
inc:
				ip+= op.size;
		}
		ofs << "\n";
	}
finis:

	ofs << "\nLABELS:\n";
	for(int i=0; i< labels.size(); ++i) {
		ofs << i << " " << labels[i].name << " " <<stringify(labels[i].value) << "\n";
		std::flush(ofs);
	}

	ofs << "\nKSTRS:\n";
	for(int i = 0; i < kstrs.size(); ++i) {
		ofs << i << "\t\"" << kstrs[i] << "\"\n";
		flush(ofs);
	}

	ofs.close();
}

void resolve_labels()
{
	ip = 0;
	while(1) {
		auto opcode = bget(ip) + HALT;
		if(opcode==HALT) goto finis;
		opcode_t& op = opmap[(yytokentype) opcode];
		if(op.opcode == LABEL) {
			int label_idx = iget();
			//ip -= sizeof(int);
			labels[label_idx].value = ip;
			//iput(add
		} else {
			ip += op.size;
		}
	}
finis:
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

	resolve_labels();

	decompile();

	ip = 0;
	while(eval1());

	print_stack();


	//disassemble();
	//eval(top_prog_node);

	return 0;
}
