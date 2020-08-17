#include <functional>
#include <iostream>
#include <stack>

#include "blang.h"

using namespace std;

extern FILE* yyin;
extern char* yytext;
//extern int yylex();

void yyerror(const char*)
{
	cerr << "ohoh spagetyo\n";
}

void note(const char* msg)
{
	cout << msg << "\n";
}

void xnote(const char* msg) {}

YYSTYPE mkint(std::string intstr)
{
	tac_t t;
	t.op = TAC_ARG;
	t.args[0] = stoi(intstr);
	tacs[tacs_idx] = t;
	return &tacs[tacs_idx++];
}
YYSTYPE mkbin(int op, YYSTYPE arg1, YYSTYPE arg2)
{
	tac_t t;
	t.op = op;
	t.args[0] = arg1;
	t.args[1] = arg2;
	tacs[tacs_idx] = t;
	return &tacs[tacs_idx++];
}

void print_tacs()
{
	puts("print_tacs:begin");
	for(int i=0; i<tacs_idx; ++i) {
		tac_t& t = tacs[i];
		printf("%d\t%p\t%d\t", i, &t, t.op); //, t.args[0], t.args[1], t.args[2]);
		for(int j=0; j<3; ++j) {
			tacarg& arg = t.args[j];
			if(holds_alternative<int>(arg)) {
				printf("\t%d", get<int>(arg));
			} else if(holds_alternative<string>(arg)) {
				printf("\t%s", get<string>(arg).c_str());
			} else {
				printf("\t%p", get<tacptr>(arg));
			}

		}
		puts("");
		//cout << i << "\t" <<  "
	}
	puts("print_tacs:end");
}

YYSTYPE join_tac(YYSTYPE car, YYSTYPE cdr)
{
	tac_t t;
	t.op = TAC_CONS;
	t.args[0] = car;
	t.args[1] = cdr;
	tacs[tacs_idx] = t;
	return &tacs[tacs_idx++];
	//cout <<"join_tac:TODO\n";
	//return nullptr;
}

YYSTYPE mkstm(int cmd, YYSTYPE arg)	
{		
	tac_t t;
	t.op = cmd;
	t.args[0] = arg;
	tacs[tacs_idx] = t;
	return &tacs[tacs_idx++];
}

//tacptr gettac(const tacarg& arg)
//{
//	return get<tacptr>(arg);
//}


stack<int> dstack;
void dpush(int v) { dstack.push(v); }
int dpop() { int v = dstack.top(); dstack.pop(); return v; }

//#define arg0 tac->args[0]
//#define arg1 tac->args[1]

int gint(const tacarg& targ) { return get<int>(targ); }
tacptr gptr(const tacarg& targ) { return get<tacptr>(targ); }
tacarg eval(tacptr tac);

tacarg eval(const tacarg& arg) { return eval(gptr(arg)); }

using binfunc = function<int(int, int)>;
int add(int a, int b) { return a+b; }
int sub(int a, int b) { return a-b; }

int binop(binfunc fn, const tacarg& arg0, const tacarg& arg1)
{
	int int0= gint(eval(arg0));
	int int1= gint(eval(arg1));
	return fn(int0, int1);
}


tacarg eval(tacptr tac)
{
	tacarg& arg0 = tac->args[0];
	tacarg& arg1 = tac->args[1];
	switch(tac->op) {
		case TAC_ADD:
			return binop(add, arg0, arg1);
		case TAC_SUB:
			return binop(sub, arg0, arg1);
		case TAC_ARG:
			return arg0;
		case TAC_PRINT:
			cout << gint(eval(arg0)) << "\n";
			break;
		case TAC_CONS:
			eval(arg0);
			eval(arg1);
			break;

	}
	return 0;
}


int main(int argc, char *argv[])
{
	tacs.reserve(10000);
	yyin = fopen(argv[1], "r");

	/*
	   while(yylex()) {
	   cout << "yytext = " << yytext << "\n";
	   }
	   */

	yyparse();
	fclose(yyin);

	print_tacs();

	eval(&tacs[tacs_idx-1]);

	return 0;
}
