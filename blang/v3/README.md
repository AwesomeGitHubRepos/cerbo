# blang v3

`blang` is a toy BASIC interpreter written in C++17 with less than 800 line of code in a single source file. 
No external tools (e.g. bison or flex) are required.
The lexing is performed by using C++ regexes. The AST (Abstract Syntax Tree) is
constructed using recursive descent.

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

## Thoughts on C++

C++ has proven to be an excellent language from which to craft an interpreter. Modern C++ has two killer features: the `lambda`, and the `variant` (standardised in C++17). Actually, the notion of "sum types" (aka "algebraic types") is still an awkward concept in C++. Polymorphism stills seems to be more work than it ought to be. I think that sum types need to be a first-class citizen in C++, with its own syntax, much like we already have with "product types" (aka "structures"). 

The standard committee has opted for a class-based approach to handling polymorphism. The intent is, presumably, to avoid cluttering up the language with yet more syntax, but the solution at the moment is trying to fit a square peg into a round hole. With sufficient force, it can be done - and `variant` is a useful hammer - but the result is not particularly elegant.

The problem we're trying to solve is to create a collection of derived classes from a common base class. There still is no way of doing it. What I really want is to be able to write something like:
```
vector<Base> collection{DerivedClass1(), DerivedClass2() ...};
```

There is no way to do it, though. Prior to C++17, you could sorta do it, by having pointers to base:
```
vector<Base *> = ...
```

This solves one problem, but creates another: how do you manage the pointers? The solution I adopted in version 2 of blang was to use `unique_ptr`s. This means that C++ manages the resources. It works, but it's ugly. As I started developing the latest version, I found I had to `move` ownership to different places.

In the end, I abandoned the idea of a base/derived classes. Syntactic types are now just ordinary classes, not derived from a base class. Instead, `variant`s are used. They are used in two distinct places:
* to represent polymorphic "atomic" type: `typedef variant<double,string> value_t`. So when you write a `let` statement, the variable assigned to it will be of type `value_t`. 
* to represent variants of symantic type:
