# blang v4

blang is a toy BASIC interpreter written in C++17. Architectural notes are in the README for [v3](../v3/README.md).



## Synopsis

**`blang [-f file]`***

`blang` reads a BASIC program from `stdin`, and executes it. Use the `-f` option to read the program from a file instead.





## Commands and functions

### `lines()`, `readln()`

`lines()` checks the availability of input from `stdin`. `readln()` reads a lines from `stdin`.

The program `cat.bas` shows how to echo `stdin` to `stdout`:

```
while lines()
        print("line:", readln())
wend
```

### `readln()`

See `lines()`.
