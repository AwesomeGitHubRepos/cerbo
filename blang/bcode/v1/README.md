# Part 1 - just get something working

Our assembly language will be particularly brittle and simple. I just want to see something working.


## Instruction set

Let's define our instruction set for the assembly language. We won't worry about jumps and conditionals yet. We just want to get something working.

| Instruction | Meaning                   |
| ----------- | ------------------------- |
| #           | Comment                   |
| 0           | halt execution            |
| p'nnn'      | push <nnn> onto stack     |
| x'cccc'     | execute instruction <nnn> |

Notes: 
* Comments begin with `#` and last until the end of the line.
* white spaces are ignored *between* instructions
* `0' stops the interpreter
* `n` represents a digit `0..9`. p'nnn' means the letter p followed by exactly 3 digits. e.g. `p321`.
* `c` represents any ASCII character. x'cccc' means x followed by 4 chars.

The stack takes 64-bit words, and you push a number onto the stack using the `p` command.

`x` means "execute a vectored instruction". The idea is that you extend the instruction set of the interpreter, and associate a 4-letter mnemonic with it. I have provided two canned instructions for illustrative purposes:
* `xhell` - which simply prints the phrase "hello world" to output
* `xemit` - which pops a value from the top of the stack, and prints it.

### Examples

The program `hello.asm` prints "hello world":
```
# simplest program possible
# just print 'hello world'
xhell
0
```

Note that it is not necessary to have white spaces between instructions. I could have simple written `xhell0` if I chose. 

Let's use the full extend of the instruction set to print "HI" to stdout. The program `hi.asm` does this:
```
# print HI to screen
p072  # ascii H
xemit # print the char to screen
p073  # ascci I
xemit
p010  # \n
xemit
0     # halt program
```

It is failry well documented. iUsing the instruction `p072`, we push the number 72 to the stack, which is the ascii value for "H". We then call `xemit`, which prints it to the console. We do likewise for "I", which has a decimal value 73, and for the carriage return, which has a value of 10. Finally, we call `0` to halt the interpreter.
