#  vim:  ts=4 sw=4 softtabstop=0 expandtab shiftwidth=4 smarttab syntax=off

import streams

var ssin = newStringStream("12  + 13")

var yylval = 1
var yytext = ""

proc yylex() =
    yytext = ""
    yylval  = 0
    while(not atEnd(ssin) and peekChar(ssin) == ' '): discard readChar(ssin)
    if(atEnd(ssin)): return
    while(not atEnd(ssin) and peekChar(ssin) != ' '): add(yytext, readChar(ssin))
    yylval = 1
    
proc scan_all() =
    while yylval != 0:
        yylex()
        echo yytext

proc num(): bool =
    for c in yytext:
        if c < '0' or c > '9': return false
    return true;

######################################################################

proc ex3_num(): bool =
    if not num(): return false
    echo "LD ", yytext
    yylex()
    return true

proc ex3(): bool =
    return ex3_num()

proc ex1(): bool =
    return ex3()


yylex()
discard ex1()

#scan_all()
