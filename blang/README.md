# blang

A toy BASIC interpreter written in C++. Its aims to be:
* a learning exercise on how to write interpeters
* simple, one file
* an experiment for possible integration into the neoleo spreadsheet

## Features/non-features

* keywords: else, for, if, let, next, print, then, wend, while
* relational operators (<, <=, >, >=, ==, !=), work like normal operators (e.g. you can say 1 < x < 5)
* logical operators can be faked ( * for AND, + for OR)
* all variables are doubles, global, and do not have to be pre-declared. No strings
* comments start with a '

## Subprojects

* [bcode](bcode/README.md) - a bytecode compiler experiment
* v2 - an attempt at a rewrite of a parser based on s-expr
* [v3](v3/README.md) - a complete BASIC interpreter in 760 lines of code
* [v4](v4/README.md) - going beyond version 3 to include a bunch of other stuff
* lisp - an interpreter for lisp copied from github

## References

* [Scheme 9 from Empty Space](http://t3x.org/s9fes/) - R4RS scheme implementation, which seems to be regarded highly
* [Small BSIC Interpreters](https://sites.google.com/site/smallbasicinterpreters/source-code) - some implementations of BASIC, with source code

* [Writing a very simple lexical analyser in C++](https://stackoverflow.com/questions/34229328/writing-a-very-simple-lexical-analyser-in-c)
