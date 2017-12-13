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
* `n' represents a digit `0..9`. p'nnn' means the letter p followed by exactly 3 digits. e.g. p321.
* `c` represents any ASCII character. x'cccc' means x followed by 4 chars.


