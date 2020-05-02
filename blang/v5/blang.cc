#include <cassert>
#include <cmath>
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

int make_kstr(char* chars)
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

typedef std::function<void()> func_t;
typedef struct { yytokentype opcode; string name; int size; func_t fn; } opcode_t;

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
		default:
			assert(false);
	}
	push(res);
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
	cout << "eval_kstr:" << idx << "," << str << endl;
	push(str);
	flush(cout);

}
const auto int1 = sizeof(int);
const auto int2 = 2*sizeof(int);

vector<opcode_t> opcodes{
	opcode_t{GOTO,	"GOTO",	int1, 	eval_goto},
	opcode_t{IF, 	"IF",	int1, 	eval_if},
	opcode_t{INTEGER, "INT",int1, [](){ push(iget());}},
	opcode_t{JREL, 	"JREL",	int1, 	eval_jrel},
	opcode_t{LABEL, "LAB",	int1, 	[](){ iget(); }},
	opcode_t{LET, 	"LET",	int1, 	eval_let },
	opcode_t{MUL, 	"MUL",	0, 	[](){ do_arith(MUL);}},
	opcode_t{PLUS, 	"PLUS",	0, 	[](){ do_arith(PLUS);}},
	opcode_t{PRINT, "PRINT",0, 	[](){ eval1(); cout << stringify(pop()) << " "; std::flush(cout);}},
	opcode_t{SUB, 	"SUB",	0,	[](){ do_arith(SUB);}},
	opcode_t{VAR, 	"VAR",	int1, [](){ push(vars[iget()].value); }},
	opcode_t{KSTR,	"KSTR", int1,	eval_kstr }
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
	ip = 0;
	while(eval1());

	cout << "Stack is: ";
	while(!stk.empty()) cout << stringify(pop()) << " ";
	cout << "\n";

	return;

}

void decompile()
{
	ip = 0;
	while(1) {
		auto opcode = bget(ip) + HALT;
		if(opcode==HALT) goto finis;
		opcode_t& op = opmap[(yytokentype) opcode];
		cout << op.name << " ";
		switch(op.opcode) {
			case INTEGER: //fal;
			case GOTO: // fallthrough
			case LABEL:
				cout << iget();
				break;
			default:
				ip+= op.size;
		}
		cout << "\n";
	}
finis:

	cout << "\nLABELS:\n";
	for(int i=0; i< labels.size(); ++i) {
		cout << i << " " << labels[i].name << " " <<stringify(labels[i].value) << "\n";
		std::flush(cout);
	}
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

	eval();

	//disassemble();
	//eval(top_prog_node);

	return 0;
}
