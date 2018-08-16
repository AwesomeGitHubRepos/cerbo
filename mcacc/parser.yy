%{
#include <iostream>
#include "scanner.h"

void trace(const std::string& s);	
%}

%token L_NEWLINE L_TEXT

%%

records :
  %empty
| records L_NEWLINE  /* a record containing just a newline. Ignore it */
| records record
;	

record : 
| fields L_NEWLINE  { trace("record newline"); }
;


fields : L_TEXT {  trace("fields:L_TEXT:" + $1 + "."); }
| fields L_TEXT { trace("1"); }
;

%%

void yyerror(std::string s)
{
	puts("yyerror called:");
	puts(s.c_str());
	puts(yylval.c_str());
	exit(1);
}

void trace(const std::string& s)
{
	std::cout << "parser:trace:" << s << std::endl;
}
