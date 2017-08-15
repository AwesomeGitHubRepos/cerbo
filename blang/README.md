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


## References

* [Small BSIC Interpreters](https://sites.google.com/site/smallbasicinterpreters/source-code) - some implementations of BASIC, with source code

* [Writing a very simple lexical analyser in C++](https://stackoverflow.com/questions/34229328/writing-a-very-simple-lexical-analyser-in-c)
