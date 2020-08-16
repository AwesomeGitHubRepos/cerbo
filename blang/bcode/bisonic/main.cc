#include <iostream>

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

	return 0;
}
