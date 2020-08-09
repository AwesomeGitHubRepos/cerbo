.SYNTAX PROGRAM

EX3= .NUMBER .OUT("LDL(" * ")")/ "(" EX1 ")" ;
EX2 = EX3 $ ("*" EX3 .OUT("MLT()")) ;
EX1 = EX2 $ ("+" EX2 .OUT("ADD()")) ;
PROGRAM = EX1 .OUT("PTOS()") ;
.END
