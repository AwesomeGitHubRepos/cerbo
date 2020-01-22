#  vim:  ts=4 sw=4 softtabstop=0 expandtab shiftwidth=4 smarttab syntax=off

import re
import os
import streams
import strutils
import system
import tables

type
    opcode_t =tuple[arg1, arg2:string]

var opcodes: seq[seq[string]]
var labels = initTable[string, int]()
var ip:int = 0
var exit_vm = false

var input_string:string
var switch:int

#var stack: seq[int]

proc tos():int =
    return switch
    #stack[len(stack)-1]

var call_stack: seq[int]



proc deln(n:int) =
    #if n <= 0: return
    input_string = input_string.substr(n)

proc eat_white() =
    while len(input_string) > 0 and (input_string[0] == ' ' or input_string[0] == '\t'):
        deln(1)

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

proc match(str:string):bool =
    if len(input_string) < len(str): return false
    let against = input_string.substr(0, len(str)-1)
    echo "against:<", against, ">"
    return str == against

proc parse() =
    for line in lines "meta.asm":
        if len(line) == 0: continue
        if line[0] == '\t' or line[0] == ' ':
            found_instr(line)
        else:
            found_label(line)

proc run_op() =
    let op = opcodes[ip]    
    ip += 1;
    let cmd = op[0]
    var arg = ""
    if len(op)>1 : arg = op[1]
    case cmd:
        of "ADR": ip = labels[op[1]]

        of "BE":
            if switch == 0: return
            echo "SYNTAX ERROR. Remainder of stream follows"
            echo input_string
            raise newException(OSError, "Syntax Error")

        of "BF": 
            if tos() == 0:
                ip = labels[arg]

        of "CL":
            echo op[1], " "

        of "ID":
            switch = 0
            if len(input_string) < 1: return
            if not Letters.contains(input_string[0]): return
            switch = 1
            var n = 1
            while(len(input_string) > n and (Letters.contains(input_string[n]) or Digits.contains(input_string[n]))):
                n = n + 1
            let m = input_string.substr(0, n)
            echo "Found ID:<", m, ">"


        of "R":
            if len(call_stack) == 0:
                exit_vm = true
            else:
                ip = pop(call_stack)

        of "TST": 
            echo "TST called"
            eat_white()
            if match(arg):
                echo "TST matched ", arg
                deln(len(arg))
                switch = 1
            else:
                switch = 0

        else: raise newException(OSError, "Unkown opcode:" & cmd)

            

proc run() =
    while not exit_vm: run_op()

parse()        

var strm = newFileStream("meta.meta", fmRead)
input_string = strm.readAll()
strm.close()
#echo input_string

run()


