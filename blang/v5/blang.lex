%option noyywrap

%{

#include <iostream>
#include <memory>
#include <string>


using std::cin;
using std::cerr;
using std::cout;
using std::string;

#include "blang.h"
#include "blang.tab.hh"

int line_number = 0;
//extern yylval;

// TODO reinstate these pattersn
// \"[^"]*\"	{ yylval = store_string(yytext); return TEXT; }
// [a-zA-Z]+ 	{ yylval = store_string(yytext); return IDENT; }
	
void set_yylval(int i)
{
	yylval = to_bvec(i);
/*
	yylval.clear();
	byte_t* arr = (byte_t*) &i;
	for(int j=0; j< sizeof(int); ++j) yylval.push_back(*(arr+j));
*/

}

int var_idx(string varname)
{
	int res;
	for(res = 0; res < vars.size(); ++res)
		if(vars[res].name == varname) return res;
	vars.push_back(var_t{varname, 0});
	return res;
}

%}

ws	[ \t\r\n]
%%

[\t\r ] // discard whitespace
\n	line_number++;
"="		{ return EQ; }
"("		{ return LRB; }
")"		{ return RRB; }
"-"		{ return SUB;}
"+"		{ return PLUS;}
"*"		{ return MUL;}
"/"		{ return DIV; }
"^"		{ return POW; }
";"		{ return SEMI;}
[0-9]+		{ set_yylval(std::stoi(yytext)); return INTEGER;}
IF		{ return IF; }
THEN		{ return THEN; }
ELSE		{ return ELSE; }
FI		{ return FI; }
JUST		{ return JUST;}
PRINT		{ return PRINT;}
[a-z]([a-z]|[0-9])*	{ set_yylval(var_idx(yytext)); return VAR; }

%%
