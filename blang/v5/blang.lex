%option noyywrap

%{

#include <iostream>
#include <memory>
//#include <map>
#include <string>
#include <vector>


using std::cin;
using std::cerr;
using std::cout;
//using std::map;
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

int get_label_id(string label)
{
	//static int id = 0;
	static std::vector<string> labels;
	// check for prexisitng label
	for(int i = 0; i< labels.size(); ++i)
		if(labels[i] == label)
			return i; 

	// not found, so add it
	labels.push_back(label);
	return labels.size();
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
GOTO		{ return GOTO; }
JUST		{ return JUST;}
PRINT		{ return PRINT;}
[a-z]([a-z]|[0-9])*	{ set_yylval(var_idx(yytext)); return VAR; }
:[a-z]([a-z]|[0-9])*	{ set_yylval(get_label_id(yytext));  return LABEL; }

%%
