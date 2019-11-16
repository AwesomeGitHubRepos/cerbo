# bbf - barebones forth

bbf is a barebones Forth written in C with the following design goals:

* small and easy to understand and extend
* suitable for MCUs (microcontrollers) and desktop use (Linux, Cygwin, etc.)

## Versions

* [v1](v1) - obsolete version
* [v2](v2) - **CURRENT** version. Changed the dictionary layout a little. Compiled words 
now embed to "cfa" (code field address) instead of the head of the word's dictionary
definition. Code should therefore run a little faster, although I haven't benchmarked it.

## Bugs

None known 

## Implementing a Forth

* WORDS should be one of the first words you implement. This will allow you to sanity-check the structure of your dictionary.

* Before you implemnt `BRANCH`, implement `[`, `]`, `HERE`, `,` to faciliate testing. Test it with a simple word: 
`: ouch 0 [ here ] 1 + dup . branch [ , ] ;`. 
The word will go into an infinite loop, but you should see it write an increasing count, showing that it works.
Similarly: `: count 5 [ here ] dup . 1 - dup ?branch [ , ] ;`

## References

* [Forth Simplicity](http://wiki.c2.com/?ForthSimplicity) - defines IMMEDIATE?, NEXTWORD, NUMBER,, COMPILEWORD, [, ].
