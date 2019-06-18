# bbf - barebones forth

## Bugs

## Implementing a Forth

* WORDS should be one of the first words you implement. This will allow you to sanity-check the structure of your dictionary.

* Before you implemnt `BRANCH`, implement `[`, `]`, `HERE`, `,` to faciliate testing. Test it with a simple word: 
`: ouch 0 [ here ] 1 + dup . branch [ , ] ;`. 
The word will go into an infinite loop, but you should see it write an increasing count, showing that it works.
Similarly: `: count 5 [ here ] dup . 1 - dup ?branch [ , ] ;`

## References

* [Forth Simplicity](http://wiki.c2.com/?ForthSimplicity) - defines IMMEDIATE?, NEXTWORD, NUMBER,, COMPILEWORD, [, ].
