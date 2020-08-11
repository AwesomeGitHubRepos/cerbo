rem an interpreter
rem 2020-08-08 mcarter started

DQ = 34 rem the double-quote 
rem print_tokens()

LABEL PROGRAM
	getchar()
	yylex()
	ok = TST(".SYNTAX") and ID() and OUT("	ADR " + star$) and STs() and TST(".END") and OUT("	END")
	print "Finished SYTNAX"
END

sub OUT(s$)
	print s$
	return true
end sub

sub ID()
	if  yytype$ <> "I"  return false
	star$ = yytext$
	yylex()
	return true
end sub

sub EX3()
	return (ID() and OUT("	CLL *")) or (STR() and OUT("	TST " + star$))
end sub

sub EX2()
	return EX3() and OUT("	EX2 BF *1")
end sub

sub EX1a()
	while TST("/") and OUT("	BT *1") and EX2() wend
	return true
end sub

sub EX1()
	return EX2() and EX1a() and OUT("	R EX1")
end sub

sub STs()
	while ID() and OUT("	ST " + star$) and TST("=") and EX1() and TST(";") and OUT("R") wend
	return true
end sub

sub STR()
	if yytype$ <> "S"  return false
	star$ = yytext$
	yylex()
	return true
end sub

sub TST(s$)
	local ok
	ok  = (yytext$ = s$)
	rem print "TST ", s$
	if ! ok return false
	yylex()
	return true
end sub
	

label yy_panic
	print "Syntax error: unexpected:", yytext$
	END


REM read all tokens
sub print_tokens()
	getchar()
	while ch <> -1
		yylex()
	wend
	print "Finished analysing"
	end
end sub

REM 	=== LEXER ===

sub yylex()
label begin
	yytext$ = ch$ 
	if white then
		getchar()
		goto begin
	endif
	ok = yy_id() or yy_key() or yy_string() or yy_unknown()
	return ok
end sub

sub yy_unknown()
	rem print "unknown:", ch$
	getchar()
	yytype$ = "U"
	return true
end sub

sub yy_id()
	if ! alpha  return false
	while true
		getchar()
		if !(alpha or digit) break
		yytext$ = yytext$ + ch$
	wend
	rem print "yy_id:", yytext$
	yytype$ = "I"
	return true
end sub

sub yy_key()
	if ch$ <> "." return false
	while true
		getchar()
		if not alpha break
		yytext$ = yytext$ + ch$
	wend
	rem print "yy_key:", yytext$
	yytype$ = "K"
	return true
end sub

sub yy_string() REM tokenize a double-quoted string
	rem print "yy_string:called:", ch
	if ch <> DQ return false
	yytext$ = ""
	while getchar() <> DQ
		yytext$ = yytext$ + ch$
	wend
	getchar()
	print "string: '", yytext$, "'"
	yytype$ = "S"
	return true
end sub



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