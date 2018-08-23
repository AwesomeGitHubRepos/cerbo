%%
[ \t\n]	(yycontinue)
;plus	(make-lexical-token 'PLUS  0 #f)
;minus	(make-lexical-token 'MINUS 0 #f)
;div	(make-lexical-token 'DIV   0 #f)
;mul	(make-lexical-token 'MUL   0 #f)
;[0-9]+	(make-lexical-token 'NUM   0 yytext)
;<<EOF>>	'MY-EOF

"+"	(my-make-token '+ #f)
"*"	(my-make-token '* '*)
"-"	(my-make-token '- '-)
"/"	(my-make-token '/ '/)
[0-9]+	(my-make-token 'NUM (string->number yytext))
<<EOF>>	'*eoi*

%%
