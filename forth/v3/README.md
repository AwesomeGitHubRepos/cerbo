# bbf - barebones forth v3

bbf is written in C and uses direct threading. It supports things like 
<code>DEFER</code>  <code>&lt;build ... does&gt;</code>, and other goodies.
It is designed to be as easy to understand as possible, and can be used as
a basis for further experimentation.

## Writing `cat` in Forth

This doesn't work properly as of 2020-01-22 because it really needs to take
in 4th files as command-line arguments. But the general gist of it is:

```
\ cat.4th
." START OF CAT" cr

: cat begin refill while tib type repeat  . "END OF CAT" cr ;
cat
```

Run it: `cat cat.4th somefile.txt | forth`


## See also:


* [bestiary](https://github.com/marcpaq/b1fipl) - single-file implementations of programming languages, which includs bbf3

* [words](words.txt)

## Glossary

**cfa** - code field address
