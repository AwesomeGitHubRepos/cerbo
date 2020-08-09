rem an interpreter
rem 2020-08-08 mcarter started

DQ = 34 rem the double-quote 

REM read all tokens
label print_tokens
	getchar()
	while ch <> -1
		gosub yylex
	wend
	print "Finished analysing"
END

label yylex
	yytext$ = ch$ 
	if white goto yy_white
	if alpha  or ch$ = "." goto yy_id
	if ch = DQ goto yy_string
	print "unknown:", ch$
	getchar()
rem label yylex_end
	return


label yy_white
	getchar()
	return
	rem goto yylex_end


label yy_id
	getchar()
	if !(alpha or digit) goto yyid_fin
	yytext$ = yytext$ + ch$
	goto yy_id
label yyid_fin
	print "ID:", yytext$
	return
	rem getchar()
	goto yylex_end

label yy_string REM tokenize a double-quoted string
	yytext$ = ""
	while getchar() <> DQ
		yytext$ = yytext$ + ch$
	wend
	getchar()
	print "string: '", yytext$, "'"
	return
	goto yylex_end


sub isalpha(ch)
	if 65 <= ch and ch <= 90 return true rem A-Z
	if 97 <= ch and ch <= 122 return true rem a-z
	return false
end sub

sub isdigit(ch)
	if 48 <= ch and ch <= 57 return true rem 0-9
	return false
end sub

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
	alpha = isalpha(ch)
	digit = isdigit(ch)
	white = iswhite(ch)

	ch$ = ""
	if ch >0 ch$ = chr$(ch)
	return ch
end sub
