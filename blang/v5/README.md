# blang5

BASIC language interpreter. 

Design features:
* C++
* uses bison/flex instead of a hand-rolled top-down parser
* creates bytecodes with a VM (virtual machine)

Language features: 
* case-sensitive. Commands must be in upper-case, variables in lower-case
* commands: GOTO, IF/THEN/ELSE/FI, PRINT



## Bugs

* GOTO to undefined label will cause a segfault.

## Examples

### Example 1: Factorial
```
fact = 1
i = 10
:loop
fact = fact * i
i = i - 1
IF i THEN GOTO :loop FI
PRINT fact
```

## Implementation details:


### Overview

The parser works by building up fragments of bytecodes, and assembling them into one big sequence of bytecodes at the end.

### Labels and gotos

The compiler doesn't know the address of the labels until the final bytecode has been constructed. It therefore has
to embed labels in the bytecode in the first instance. The bytecode is then scanned for the labels to resolve the GOTOs.



## References

* [Augmented Backus–Naur form](https://en.wikipedia.org/wiki/Augmented_Backus%E2%80%93Naur_form)
* [Eliminating left-recursion](http://www.d.umn.edu/~hudson/5641/l11m.pdf) - PDF
* [Oil blog](http://www.oilshell.org/blog/) - blog on writing a UNIX shell
* [pascal lex](https://github.com/westes/flex/blob/master/examples/manual/pascal.lex)
* [Using std::variant as YYSTYPE/yylval in gnu bison for C++17](https://mcturra2000.wordpress.com/2018/05/18/using-stdvariant-as-yystype-yylval-in-gnu-bison-for-c17/)
