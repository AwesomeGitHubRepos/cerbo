# blang v3

`blang` is a toy interpreter

Completed 12-Mar-2018

## Features

* function declarations via def ... fed. Functions can be used recursively
* while ... wend loops
* if ... then ... else ... fi loops
* let assignment
* print command
* types: strings and doubles
* respects operator precedence
* variables don't have to be declared before use

## Examples

Compute the infamous factorial, which exercises much of the code base:

```
def fact(n)
        if n > 0 
        then
                let x := n * fact(n-1)
        else
                let x := 1
        fi
        x
fed

print("fact(10)=", fact(10))
```


