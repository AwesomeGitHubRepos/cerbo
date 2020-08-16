rem an interpreter
rem 2020-08-08 mcarter started

DQ = 34 rem the double-quote 
rem print_tokens()

LABEL PROGRAM
	getchar()
	yylex()
	ok = TST(".SYNTAX") and IDO("	ADR " + star$) and STs() and TSTO(".END", "	END")
	print "Finished SYTNAX"
END

sub OUT1()
	return TSTO("*1", "GN1") or TSTO("*2", "GN2") or TSTO("*", "CI") or STRO("CL *")
end sub

sub OUTPUTa()
	while OUT1() 
	wend
	return true
end sub

sub OUTPUT()
	return ((TST(".OUT") and TST("(") and OUTPUTa() and TST(")")) or (TSTO(".LABEL", "LB") and OUT1())) and OUT("OUT")
end sub

sub OUT(out$)
	print out$
	return true
end sub

sub ID()
	if  yytype$ <> "I"  return false
	star$ = yytext$
	yylex()
	return true
end sub

sub IDO(out$)
	if not ID() return false
	print out$
	return true
end sub

sub EX3()
	ok = 	IDO("	CLL *") or STRO("	TST " + star$) or TSTO(".ID", "ID") or TSTO(".NUMBER", "NUM") or TSTO(".STRING", "SR") or (TSTO("(") and EX1() and TSTO(")")) or TSTO(".EMPTY", "SET") or TSTO("$", "LABEL *1") and EX3() and OUT("BT +1") and OUT("SET")
	return ok
end sub

sub EX2a()
	while (EX3() and OUT("BE")) or OUTPUT()
	wend
	return true
end sub

sub EX2()
	return ((EX3() and OUT("	EX2 BF *1")) or OUTPUT()) and EX2a() and OUT(".LABEL *1")
end sub

sub EX1a()
	while TSTO("/", "	BT *1") and EX2() wend
	return true
end sub

sub EX1()
	return EX2() and EX1a() and OUT("	R EX1")
end sub

sub STs()
	while IDO("	ST " + star$) and TST("=") and EX1() and TSTO(";", "R") 
	wend
	return true
end sub

sub STR()
	if yytype$ <> "S"  return false
	star$ = yytext$
	yylex()
	return true
end sub

sub STRO(out$)
	if not STR() return false
	print out$
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
	
sub TSTO(s$, out$)
	if ! TST(s$) return false
	print out$
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
