# blang v4

blang is a toy BASIC interpreter written in C++17. Architectural notes are in the README for [v3](../v3/README.md).



## Synopsis

**`blang [-f file]`**

`blang` reads a BASIC program from `stdin`, and executes it. Use the `-f` option to read the program from a file instead.





## Commands and functions

### `cut(str, n)`

Return the `n`th field of `str`, using a tab field separator. `n` starts from 1.
```
cut("foo	bar", 1) ' foo
cut("foo	bar", 2) ' bar
cut("foo	bar", 3) ' empty string
```
All whitespaces above are tabs.


### `FmtNum(n, str)`

Format a number `n` according to format scifier `str`. E.g.
```
FmntNum(42, "%04.0f") ' 0042
```

### `lines()`, `readln()`

`lines()` checks the availability of input from `stdin`. `readln()` reads a lines from `stdin`.

The program `cat.bas` shows how to echo `stdin` to `stdout`:

```
while lines()
        print("line:", readln())
wend
```

### `lmatch(targ, src)`

Tries to match `targ` against `src` from the left. Eg.
```
lmatch("foo", "foobar") ' 1
lmatch("foo", "bar)     ' 0
lmatch("foo", "barfoo") ' 0
```
### `readln()`

See `lines()`.

### `TAB()`

Return the tab character as a string.
