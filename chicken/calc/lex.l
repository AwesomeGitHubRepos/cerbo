%%
[ \t\n]	(yycontinue)
[a-z]+	(list 'WORD yytext)
[0-9]+	(list 'NUM  yytext)
<<EOF>>	'MY-EOF

%%
