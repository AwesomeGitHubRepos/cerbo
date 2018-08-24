
%%
[ \t\r\n]	(yycontinue)
"("	(simple-token 'LRB)
")"	(simple-token 'RRB)
"+"	(simple-token '+)
"*"	(simple-token '*)
"-"	(simple-token '-)
"/"	(simple-token '/)
"^"	(simple-token 'POW)
";"	(simple-token 'SEMI)
JUST	(simple-token 'JUST)
[0-9]+	(my-make-token 'NUM (string->number yytext))
<<EOF>>	'*eoi*

%%
