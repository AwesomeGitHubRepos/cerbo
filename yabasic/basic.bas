rem an interpreter

while (getchar() > -1) 
	if(white) goto skip
	yytext$ = ch$
	while ! iswhite(getchar())
		yytext$ = yytext$ + ch$
	wend
	print "found token:", yytext$
	
label skip
wend


sub iswhite(ch)
	if ch =  9 return true 	rem \t
	if ch = 10 return true 	rem \n
	if ch = 13 return true 	rem \r
	if ch = 32 return true 	rem space
	return false
end sub
	
libc$ = "libc.so.6"
sub getchar()
	ch =  foreign_function_call(libc$, "int", "getchar") 
	white = iswhite(ch)

	ch$ = ""
	if ch >0 ch$ = chr$(ch)
	return ch
end sub
