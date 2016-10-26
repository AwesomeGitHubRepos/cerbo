/* a shlex-like scanner 
created by mcarter
http://flex.sourceforge.net/manual/Patterns.html
flex toke.lex

for clues and enhancements on processing strings,
search for `quoted strings' at 
http://flex.sourceforge.net/manual/Start-Conditions.html
*/

%{
  //#include <readline/readline.h>
    #include "stats.h"
    #include <stdlib.h>
  // void yyerror(char *s);
 void yyerror(char *s) {
          printf("%d: %s at %s\n", yylineno, s, yytext);
    }
    char *p;
   //extern YYSTYPE yylval;




%}



DQUOTE "\""
HASH "#"
ALPHA [:alphanum:]
C1    [0-9a-zA-Z]
WHITE [ \t\r]
LW    ^[ \t]*
/* VCHAR (C1|"-"|".") */
VCHAR [0-9a-zA-Z]|-|"."|"_"|"?"|":"|";"|"+"


%x str

%%

                 static int MAX_STR_CONST = 1024;
                 char string_buf[MAX_STR_CONST];
                 char *string_buf_ptr;

\"      string_buf_ptr = string_buf; BEGIN(str);
     
<str>\"        { /* saw closing quote - all done */
  BEGIN(INITIAL);
  *string_buf_ptr = '\0';

  p=(char *)calloc(strlen(string_buf)+1,sizeof(char));
  strcpy(p,string_buf);
  yylval.string=p; 
  return STRING;

 }

<str>[^\"]+       {
                 char *yptr = yytext;
     
                 while ( *yptr )
                         *string_buf_ptr++ = *yptr++;
                 }


{HASH}[^\n]*"\n" /* eat up comments */

[+-]?[0-9]+(\.[0-9]+)? { yylval.dval = atof(yytext) ; return DOUBLE ;}

[a-z]+ { return CMD;}

 {VCHAR}+         {  p=(char *)calloc(strlen(yytext)+1,sizeof(char));
	strcpy(p,yytext);
	yylval.string=p; return STRING;}



^[ \t\r]*"\n"    
{WHITE}+        /* eat up whitespace */
"£"             /* drop the pound sign */
"\n"            { return NEWLINE ;}
.               { printf("Uncaught: %c\n", yytext[0]); 
   yyerror("invalid character");}



%%
int yywrap(void) {
    return 1;
}


       /*
int main( int argc, char **argv )
{ 
  puts("2");
yylex() ;
}
       */
