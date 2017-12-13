# bcode

Let's build a byte-code compiler.

## Motivation

There are several factors that have led me to writing "bcode", my attempt at making a byte-code compiler:

* my general interest in compiling technology
* my involvement with `neoleo`, a spreadsheet that I forked from `oleo`, which is GNU's spreadsheet program that has not been updated for many years. Part of the program parses user formulae, compiles and decompiles them into byte-codes as required. Lex and yacc are used. `Oleo` was originally written in C, and I was interested in converting it to C++. The workings of the compiler and parser were, and to some extent still are, a bit of a mystery to me. I figured that there must be a better way to do it.
* my involvement with [`ultimc`](https://github.com/blippy/ultimc). `Ultimc` is a project that I created to work in conjunction with [Ultibo](https://ultibo.org/), a unikernel for the RPi (Raspberry Pi). I figured that it would be nice to have a scripting environment for the unikernel, and set about creating `Glute` ("gluing utility", although I like the name because it was vaguely rude). Although it currently "works", it can only deal with commands. You cannot perform branching, or define functions.
* I was inspired by Jack Crenshaw PhD's series of articles, [Let's build a compiler](https://compilers.iecc.com/crenshaw/) to try my own take on the subject. Jack creates a compiler that spits out Motorola assembly. The assembly can then be compiled into an executable, assuming that you have access to the Motorola chipset
* J.P. Bennett's book "Introduction to compiling techniques - a first course using ANSI C, LEX and YACC" was of interest to me. I bought the book in 1996. I never made my way through all of it, although I occasionally dipped in, latterly more than formerly. I am glad I bought the book, as it is a pleasant introduction to compiling techniques.

Bennett's work adopts the typical approach to writing a compiler:
1. define a lexer using lex. He discusses ad hoc lexing techniques, where you basically roll-your-own lexer
2. analyse the syntax using yacc. He also discusses top-down parsing.
3. produce an intermediate representation of the compiled code. He chooses an idealised assembly language
4. the idealised assembly language or intermediate code can then be compiled to native machine code. Bennett actually creates an interpreter for the assembly language, which is a perfectly reasonable choice.

## Method of attack

I have this to say on compiler construction tools:
* I would rather avoid code generation. Ideally, the code should just work, without any need for tool intervention. I think a lot of the drawbacks come from the limitations of C itself
* they're too complicated. This is a contentious point, I know. Everybody's mileage varies on this. You have to learn them, and it's difficult to know if they create more problems than they solve
* the tools don't especially fit harmoniously together. I always feel that it's like trying to push two magnets together with the same polarity head-to-head. You can do it with sufficient force, but I would rather not.
* they seem more designed to the C era of programming. I want to write in C++, where encapsulation is better, and I feel I have more certainty about computer memory hygiene (no leaks). I noticed that with `neooleo`, for example, it is difficult to reason about memory. I have ideas about how it could be better, although it is difficult to realise them given the nature of the tools I have

I think lex and yacc can be considered obsolete for the following reasons:
* Rob Pike, of Go, Unix and Plan 9 fame, seems to prefer hand-rolled lexers
* hand-rolled lexers are OK, but I think they require finnicky bookkeeping as to token state and the state of the input stream
* if you don't want to go down the lexing route, and hand-rolling a very low-level lexer is too much fiddle for you, and you are willing to sacrifice some efficiency, then you can use C++ regexs. By defining a list of them, and checking against them, you can effectively achieve what lex does, but without the hassle of using a separate tool. My `blang` project uses such an approach, and I think it completely obviates the need for lex
* nearly all compiler writers today seem to use recursive descent compilers, having abandoned yacc. 
* recursive-descent compilers also fit in well with humans intuitvely

I have need of an interpreter, as opposed to a compiler. I am willing to sacrifice speed, and I am not looking to perform any optimsations. I had originally thought that a good idea would be to avoid writing any kind of byte-code compiler, and just walk through a parse tree as needed. I think it could be reasonably efficient. One of the real problems with compilers is that they rely on a lot of different "types". Types include things like integers versus strings, or function blocks versus arithmetic. The compiler needs to work with variants of data types. This is commonly called ADTs (Abstract Data Types), or Sum Types. This is in contradistinction with Product Types, or more commonly called "the struct". Product Types are widely supported in many languages. ADTs rarely feature in language designs. The notable expections are programming languages like Haskell, which excels at using ADTs. Lisp-like languages are also well-suited to ADTs, as they can deal with data representations dynamically. For a language like C++, it always felt like trying to put a square peg in a round hole. The latest version of C++ 17 does actually provide the `variant` type. Finally! As of writing (Dec 2017), full C++ compilers have not yet made their way into popular Linux distributions, though. I also wonder if C++'s solution is entirely satisfactory. It has chosen an object-oriented approach to dealing with variants, whereas I wonder if they would best be dealt with by syntactic extension. Time will tell.

Given the above factors, I am now think that byte-compiling is an idea that I want to explore. So my idea is that the parser, instead of constructing a parse tree, just spits out byte code.  In Tcl, as the saying goes, "everything is a string". It is a data-hiding mechanisem. I think it is a good approach for compilers, except that for "string", read "series of bytes".

So this is the approach I will adopt here:
* instead of trying to compile a full-blown language, I will try to write a compiler for a simple assembly language. And I'm not kidding here, the parsing should be as simple as possible
* that assembly will compile to byte-code for an "idealised" stack machine
* the byte-codes will be interpreted

I think this is a good approach. You can compile a fully-fledged language by emitting those assembly instructions. The nature of writing a top-down parser is that you can generate assembly instructions in postfix form, and you do not have to construct a syntax tree. The tree is implicit in the top-down recursion. You don't have to construct a tree explicitly. This is great, because it means that you don't have to store variant-type intermediate structures, you just emit assembly code as you go, in the manner adopted by Crenshaw.

## The code

I am trying to write my assembler piecemeal, creating features as we go. I do hope I make it to the end. So here are the parts:

[Part 1](v1/README.md) - halting, pushing and extensible execution, no branching
