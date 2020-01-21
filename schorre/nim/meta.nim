#  vim:  ts=4 sw=4 softtabstop=0 expandtab shiftwidth=4 smarttab syntax=off

import tables

var opcodes: seq[int]

var labels = initTable[string, int]()

proc found_label(line:string) =
    labels[line] = len(opcodes)
    echo "found label:", line, "."

proc found_instr(line:string) = 
    return


proc top() =
    for line in lines "meta.asm":
        if len(line) == 0: continue
        if line[0] == '\t' or line[0] == ' ':
            found_instr(line)
        else:
            found_label(line)

top()        

