delimiter	[ 	\r\n]
whitespace	{delimiter}+
ascii_char	[^\"\n]
escaped_char	\\n|\\\"
text		\"({ascii_char}|{escaped_char})*\"
digit		[0-9]
variable	[a-z]([a-z]|{digit})*

%%
{whitespace}	(yycontinue)
"#"[^\n]+	(yycontinue)
{variable}	(my-make-token 'VAR yytext)
":="		(simple-token 'ASSIGN)
"("		(simple-token 'LRB)
")"		(simple-token 'RRB)
"+"		(simple-token '+)
"*"		(simple-token '*)
"-"		(simple-token '-)
"/"		(simple-token '/)
"^"		(simple-token 'POW)
";"		(simple-token 'SEMI)
","		(simple-token 'COMMA)
IF		(simple-token 'IF)
THEN		(simple-token 'THEN)
ELSE		(simple-token 'ELSE)
FI		(simple-token 'FI)
JUST		(simple-token 'JUST)
PRINT		(simple-token 'PRINT)
{digit}+	(my-make-token 'NUM (string->number yytext))
{text}		(my-make-token 'TEXT yytext)
<<EOF>>		'*eoi*

%%
