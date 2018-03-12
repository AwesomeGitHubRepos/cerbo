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

## What I think blang is good for

* the code base is tiny, making it an excellent learning tool for people who want to write their own interpreter
* I think it would make an excellent embedded language. I already had this in mind for my "neoleo" spreadsheet project
* I am keen on exploring the idea of using blang as the basis of a shell. Parsing would have to be redone, but the inspiration is there. I would not expect a shell to be based on the BASIC language. the kind of thing I had in mind is bash meets Rexx, Awk and possibly m4 and sed. I want my bash to support floating point. I'm not sure how the design would work. Should it take a library approach a la Rexx, integrate key features like pattern matching, or provide some kind of (hygenic?) macro facility from which to build utilities like awk?

I could see a number of ways of extending the language:
* adding associative arrays as an atomic type. Lua calls them "tables", and they seem to be a loved notion in Lua. Brian Kernighan calls them "the Swiss Army Knife of data structures". And who am I to discount a programming God?
* maybe add booleans as an atomic type. I don't see that as a priority, though, as logic can be faked with doubles
* add an atomic type `Empty`. This is so that variables can be coerced to the "relevant" type as required. So, Empty coerced to a number would be 0. coerced to a string, it would be "".
* many more primitive functions could be added, like `concat()`, to concatenate strings together. An `input()` statement would be another one. Adding functions is trivial, though, "and left as an exercise for the reader". The heavy lifting has already been done: lexing, parsing and evaluating of code. 
* add commands such as `goto`, `break` and `return`. I think that this would actually be quite difficult, because each statement is unaware of its environment. How would premature exits be achieved at execution time?
* more type checking. The type checking in blang is slack. More could be done to ensure the correctness of the source program.


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
* to represent variants of symantic type: `typedef variant<Expression,Def,If,Let,For,While> Statement;`, and `class Factor { public: char sign = '+' ; variant<value_t, Expression, FuncCall, Variable> factor; };`

Dispatching on variants is not *too* bad using  `std::visit`, but it's nowhere near as neat as Haskell's solution (but then, what is?). Now that I mention Haskell, it seems to me that what C++ could really use is the notion of Haskell's `typeclass`. I have long considered that the perfect programming language would be some kind of blend between Haskell and C++. 

In the end, I decided on dispatching on the different types of a variant, rather than using a visitor. 

Perhaps if I were to reimplement my code, I would make each syntactic element a derived class of Syntax, which would have two virtual members: `make()` and `eval()`. I would still keep the variants as they are. It is just that structuring the code this way ensures that `make(0` and `eval()` are implemented in the derived class.

## Architectural notes

I am, overall, pleased as to how my code is structured. I much prefer it over version 2 of blang. I didn't like it's use of unique pointers. It was an ugly compromise at the time. With the advent of C++17, I have a better solution. Version 2 performed interpretation on-the-spot. This mostly works until you decide that you need to be to define functions. Then what do you do?

With version 3, I solved the dilemma by adopting the conventional approach of having a parsing phase (what Lisp would call the reader) where the AST is built, and an evaluation phase, which actually executes the code. 

Traditionally, interpreters create byte-codes, and then execute those. Blang does not do this. It doesn't need it. Blang just carries the tree around. It works, I don't think it is provably slower than using byte-codes, although it may use up more space, and it works. I am still interested in the possibility of using byte-code, but I wonder if it just creates more complexity. Mode code to write, more code to understand, more code to debug.

A simplification I would perform is with the precdence rules. In recursive descent, the rules are expressed as follows:
```
// I extend BNF with the notion of a function, prefixed by &
// {} zero or more repetitions
// [] optional
&M(o, X) --> X {o X} // e.g. &M(("+"|"-"), T) --> T { ("+"|"-") T }
E --> &M(( "<" | "<=" | ">" | ">=" | "==" | "!=" ), R)
R --> &M(( "+" | "-" ), T)
T --> &M(( "*" | "/" ), F)
F --> ["+"|"-"] (v | "(" E ")")
```

There are a lot of intermediate types to all this, which clutters up the code. I would probably decide to dispense with the intermeidate stages, and try to call a function like:
```
make_expression(tokes, { {"<", "<=", ...}, {"+", "-"}, {"*", "/"}})
```
What I propose would happend is that `make_expression()` would try to perform repetitions on the top level of operators passed in its second argument. If it couldn't do that, it would call itself on the remainder of the operators. When it ran out of operators, it would be down to the level of  `make_factor()`. `make_factor()` could call `make_expression()` on the full set of operators if it encountered a parenthesised expression. 

That would shrink the code base, and make things a little more readable, assuming that the maintainer understood the purpose of the recursion. Adding precedence levels would then be trival. You could even create new precedences and infix operators at run-time. Alternatively, if one were to create a "blue skies" language, one might decide to dispense with the whole idea of operator precedence anyway. 

## References

* [HelenOS](http://www.helenos.org/)
* [Irie Pascal grammar](http://www.irietools.com/iriepascal/progref534.html)
* [LLVM: Implementing a Language](https://www.gitbook.com/book/landersbenjamin/llvm-implementing-a-language/details)
* [Mathematicsl Expression Parser Using Recursive Descent Parsing](https://www.codeproject.com/Articles/318667/Mathematical-Expression-Parser-Using-Recursive-Des)
* [Parsing Expressions by Recursive Descent](https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm#classic)
* [Prolog.c: a simple Prolog interpreter written in 200 LOC of C++](https://news.ycombinator.com/item?id=12193694)
* [Simple precedence grammar](https://en.wikipedia.org/wiki/Simple_precedence_grammar)
* [Why Create a New Unix Shell?](http://www.oilshell.org/blog/2018/01/28.html)
* [Write your own Operating System](https://www.youtube.com/channel/UCQdZltW7bh1ta-_nCH7LWYw/videos)
