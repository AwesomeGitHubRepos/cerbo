#  vim:  ts=4 sw=4 softtabstop=0 expandtab shiftwidth=4 smarttab syntax=off

import streams

var ssin = newStringStream("12  13")

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
    return yytext == "12"

proc ex1(): bool =
    return num()


discard ex1()

scan_all()
