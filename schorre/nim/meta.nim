#  vim:  ts=4 sw=4 softtabstop=0 expandtab shiftwidth=4 smarttab syntax=off

import re
import strutils
import system
import tables

type
    opcode_t =tuple[arg1, arg2:string]

var opcodes: seq[seq[string]]

var labels = initTable[string, int]()

proc found_label(line:string) =
    labels[line] = len(opcodes)

proc found_instr(line:string) = 
    let args = line.split(re"\s+", 2)
    if len(args) == 2:
        opcodes.add(@[args[1]])
    else:
        var arg:string = args[2]
        if arg[0] == '\'':
            arg = arg.substr(1, len(arg)-2 )
        opcodes.add(@[args[1], arg])


proc parse() =
    for line in lines "meta.asm":
        if len(line) == 0: continue
        if line[0] == '\t' or line[0] == ' ':
            found_instr(line)
        else:
            found_label(line)

parse()        


echo opcodes
