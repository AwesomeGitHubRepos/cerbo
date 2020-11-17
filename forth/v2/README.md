# bbf - barebones forth v2

Indirect threading is used in this version. That is to say that it uses pointers to locations that in turn point to machine code. This is in contrast to
v1, which stored the address of the dictionary header.

## Metaprogramming

 Note that v3 of bbf makes things a lot simpler to do (qv)

The best way to start off is not to use macros. Suppose we want to print out a countdown. Here's how we might do it

```
: x 5 [ here ] dup .  1 - dup ?branch [ , ]  drop ;
x \ outputs: 5 4 3 2 1
.s \ just confirm that the stack is empty
```

Let's try to abstract away the loop structuring. Like so:

```
: begin 	here ; immediate
: ?again 	compile ?branch , ; immediate
\ Then we can write
: y 		5 begin dup . 1 - dup ?again drop ;
y
.s
```

Actually, bbf already defines begin and ?again, so there's no need to define it ourselves.

Let's take it one step further. Suppose we didn't like the immediate word ?again, and wanted to alias it. I've gotten into all sorts of contortions trying to make it work without crashing, and I found the easiest solution was to create a convenience word, like so:

```
' ?again constant '?again 
```

Then I can define the word I actually want:

```
: unless '?again execute ; immediate
```

Then use it in the intuitive way:

```
: z     5     begin dup . 1 - dup unless        drop ;
z
.s
```



## Glossary

**cfa** - code field address

## See also

* [words](words.md)
