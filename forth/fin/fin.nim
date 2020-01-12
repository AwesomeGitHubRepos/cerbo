# vim: set expandtab shiftwidth=4 tabstop=4

#import io
import streams
import tables


#var f : File;

var stack = newSeq[int]()

proc k() = # read key
    stack.add(int(stdin.readChar()))
proc e() = # emit char
    let ch:char = char(stack[len(stack)-1])
    stack.delete(len(stack)-1)
    var s:string = " "
    s[0] = ch
    stdout.write(s)

var dict = initTable[char, proc()]()
dict['e'] = e
dict['k'] = k

var ss = newStringStream(":l k e; r")


proc eat_white() = 
    while(ss.peekChar() == ' '): 
        discard ss.readChar()

proc interpreter() =
    eat_white()
    let ch = ss.readChar()
    case ch:
        of ':':
            let cmd = ss.readChar()
            echo "found colon"
        else:
            return

interpreter()
