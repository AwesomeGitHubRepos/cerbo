%{
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <variant>

#include "blang.h"

using std::cout;
using std::endl;

//extern int yylex();
extern int yyparse();
extern FILE* yyin;

void yyerror(const char* s);
%}

//%skeleton "lalr1.cc"


/*
%union {
	double uval;
	//std::shared_ptr<std::string> ustring;
	std::string* ustring;
}
*/


//%language "c++";
//%define api.value.type variant;
//%define api.value.type union;
//%token<dval> T_DBL;
//%token<std::string> IDENT;
//%type<std::string> foo

//%token<std::string> IDENT "identifier";
//%token<std::string> STRING "string";
//%token<int> STRING

%token IDENT PLUS STRING


///////////////////////////////////////////////////////////////////////
%%

foo: 
   IDENT PLUS IDENT{ 
   
cout << "Computer says: IDENT:" 
<< strvec.back()
<< "\n" ; 

	std::string catted =  strvec[$1] + strvec[$3];
	cout << "concat: " << catted << "\n";
	//$$ = std::string("string:666");
	//$$ = std::string("string:666");

}
;

///////////////////////////////////////////////////////////////////////
%%
