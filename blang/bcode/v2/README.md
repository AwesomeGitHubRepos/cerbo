# Part 2 - adding labels and branches

## New instructions

In this part, I will add two new instructions:

| Instruction | Meaning                   |
| ----------- | ------------------------- |
| L'c'        | add address label         |
| <'c'        | jump to label if negative |

As I write, I am increasingly coming to the realisation that this byte-compiler is an inferior implementation of Forth. It is dawning on me that Forth is, indeed, the perfect way to implement what I am trying to achieve. Nevertheless, I shall continue down the path I am going, as it seems to be good enough. The instructions embedded in the bin.out file do provide useful clues that I have coded the design correctly.

If I had gone down the road leading to Forth, I might have eskewed the whole idea of adding the two instructions above. Forth has some clever tricks where it pushes values to a return stack, and pops them off without the need for labels. I have decided to be less clever.

`L'c'` creates a label. I have allowed the creation of 256 labels, corresponding to a char byte.

`<'c'` causes a jump to the label if the value on the stack is negative.

## Ancillary functions

In order to make use of `L` and `<`, I need to use create some handing functions:

* `dupe` - which duplicates the top of the tope of the stack
* `incr` - increment the top of the stack by 1
* `subt` - subtract the top two items from the stack

The implementation of these functions is easy enough:

```
void dup()
{
        push_stack(stk.top());

}

void incr()
{
        stk.top() += 1;
}

void subt() // a b -- a-b
{
        int64_t tmp = pop_stack();
        stk.top() -= tmp;

}
```

We must remember to add them to our list of callable functions:
```
vector<func_desc> vecfns = {
        {"dupe", dup},
        {"emit", emit},
        {"hell", hello},
        {"incr", incr},
        {"subt", subt}
};
```

## Implementing labels and jumps

Define the labels as follows:
```
// The addresses of labels you create via the L command
uint8_t labels[256];
```

This is an array of mapping the label name to its address in the bytecode. This is where we jump to. Note the following:
* we are assuming that the complete set of byte-codes can take a maximum of 256 bytes (because we define it as `uint8_t`). A more reasonable assumption would be to assume that the byte-codes will take up 16-bit addresses (so we should use `unit16_t`). This would allow for 64k of code, which should be more than enough for many applications. If you wanted to be more liberal, you might aim for 32-bit or 64-bit addresses.
* you should not re-use labels. The program does not check for this, but it is almost certainly not what you want to do.
* it is entirely likely that we will want to make a forward jump, i.e. jump to a label that is defined later in the code. So we cannot fill in jumps as we go; we should complete the parsing of the code and them "resolve" the labels at the end


So we need a label reference structure:
```
// what addresses refer to those labels
typedef struct { 
        uint8_t label_name; 
        uint8_t position;
} lref_t;

```
which maps label names to their position in the byte code. As we parse the program, we add label references as necessary into the vector defined as follows:
```
vector<lref_t> label_refs;
```

During "resolution time", we fill in the references to the labels, i.e. the jump instructions, with the values we need. This is done via the function `resolve_lavels()`, defined as follows:
```
void resolve_labels(bytes &bcode)
{
        constexpr bool debug = false;

        for(const auto& lref:label_refs) {
                bcode[lref.position] = labels[lref.label_name];
                if(debug) {
                        cout << "resolve_labels:name:" << lref.label_name 
                                << ",label position:" << int(labels[lref.label_name])
                                << ",ref position:" << int(lref.position)
                                << "\n";
                }
      }
}
```

The compile-time operation switch statement for encountering a label is as follows:
```
case 'L':
	labels[prog[++i]] = bcode.size();
        break;
```
This is saying that the current byte-code position (`bcode.size()`) needs to be stored in the set of labels. The name of the label to be used is determined by `prog++i]`. Note that we do not need to create any byte-codes for a label at this point.

The compile-time behaviour of the "jump negative" instruction is:
```
case '<':
	pushchar(bcode, '<');
	label_refs.push_back({prog[++i], (uint8_t) bcode.size()});
	pushchar(bcode, '?'); // placeholder for an address to be resolved later
	break;
```
We push `<` to bcode (i.e. the byte-code) so that it will perform the relevant test and jump as necessary, and store the label that we want to jump to in the `label_refs` variable, for later resolution. We also need to push another byte which will store the address. The code above does pushes a question-mark. If you wanted to implement a 16-bit machine, you would need to push two "?"s.

`resolve_labels()` is called after we have finished processing the user program.

In terms of running the byte-code, there is nothing to do as regards the labels. They have been resolved into the jump command. The runtime behaviour for the "jump-negative" instruction is as follows:
```
case '<': // jump if negative
	{
		auto v = pop_stack();
		++pc;
		if(v<0) 
			pc = bcode[pc];
		else
			++pc;
	}
	break;
```
We pop a value off the top of the stack, as we want to know if it is negative. We move the program counter one byte along, so that it points to the location where the label is. If what was on the stack was indeed negative, we reset the program counter to the label address. Otherwise, we advance the program counter beyond this address, ready to execute the next instruction.

## Some examples

Let's test out some of the simple commands that we created first. This is the contents of `test1.asm`:
```
# basic test of dupe, ince, subt
#      stack
p000   # 0
xincr  # 1
p010   # 1 10
xsubt  # -9
xdupe  # -9 -9
0
```
We are just doing simple stack manipulation here: putting 0 on the stack, adding 1, putting 10 on the stack, performing a subtraction so that the value -9  (1-10 = -9) is on the stack, and then duplicating it.

A more taxing example is if we try to print out `ABCDE` to stdout. Here is the code in `atoe.asm`:
```
# print 
p065 # A
L0   # create label 0
xdupe
xemit
xincr
xdupe
p070 # F
xsubt
<0 # if negative, jump to label 0

# print newline
p010
xemit

0
```

Here's the sequence of events:
1. `p065` : we start by putting 'A' on the stack. 
2. `L0` : we add a label '0', which is the point we want to jump to. 
3. `xdupe xemit` : print 'A' to the screen. 
4. `xincr` : 'A' becomes 'B'
5. `xdupe xdupe p070 xsubt` : find the difference between the top of the stack ('B') and the letter 'F'.
6. `<0` : If it's negative, then jump to the position of `L0`
7 `p010 xemit` : print a newline character

The expected output is
`ABCDE`


## In summary

Inevitably when adding functionality, it is easy to make mistakes. So testing is necessary. The code is only 270 lines long, which is manageable. And we are much of the way there to producing a useful byte-code compiler and interpreter. If you wanted to extend it to be a 16-bit or 32-bit "instruction set", then care would be required.

What we need to do next is embed strings.
