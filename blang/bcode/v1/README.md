# Part 1 - just get something working

Our assembly language will be particularly brittle and simple. I just want to see something working.


## Instruction set

Let's define our instruction set for the assembly language.

| Instruction | Meaning                   |
| ----------- | ------------------------- |
| #           | Comment                   |
| 0           | halt execution            |
| p<nnn>      | push <nnn> onto stack     |
| x<cccc>     | execute instruction <nnn> |

Comments
