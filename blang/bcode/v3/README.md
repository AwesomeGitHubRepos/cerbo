# Part 3 - strings

In this part, we allow strings, which we delimit by a single quote ('). We do not implement escaping mecahnisms.



The bytecodes are now made public near the top of the file:
```
typedef vector<uint8_t> bytes;
bytes bcode; 
```
This is necessary because in order to do something like print a string, we need to know its location in the byte-code.

We also change the stack from from a C++ stack to a deque, which allows access to the stack starting from the front as well as the back. We do this so we can print a debug trace of the stack without having to pop elements off the stack:
```
void print_stack()
{
        cout << "Stack contents:";
        for(auto it = stk.begin(); it != stk.end(); ++it)
                cout << " " << *it;
        cout << "\n";
}
```
Functions that make use of the stack need a change in their internal calls. The need to call `push_back()` instead of `push()`, and `pop_back()` instead of `pop()`. 

## New executable functions


For convenience, we define the counterpart of `incr`:
```
void decr()
{
        stk.back() -= 1;
}
```

To print a string:
```
void print_string()
{
        auto len = pop_stack();
        auto pos = pop_stack();
        for(auto i=0; i< len; ++i)
                cout << bcode[pos+i];
}
```
The function assumes that the byte-code position of the string, and its length, is on the stack when it is called. We will have to generate the corect byte code to make this happen.

The two functions must be added to `vecfns` in the usual way:
```
vector<func_desc> vecfns = {
        {"decr", decr},
	...
        {"prin", print_string},
	...
};
```

## Changes to the byte-code compiler

Strings are denoted by enclosing characters in single quotes, like so: `'hello world'`. We introduce a new instruction, `j`, to perform an uncoditional jump. We also add a new instruction, `>`, which is a jump on stack positive. It is implemented in a very similar way to '<'. We do not need to do anything special at the compilation stage for `>`, and can fold it into the case for `<`.

Handling strings at the compilation stage is the most complicated code we have introducted so far:
```
case '\'': // strings
	{
		pushchar(bcode, 'j'); // push an unconditional jump instruction
		auto p0 = bcode.size(); // remember where the jump address has to be inserted
		pushchar(bcode, '?'); // reserve space for the position of end of string
		while(prog[++i] != '\'') bcode.push_back(prog[i]);
		++i; // pace the program pointer beyond the '
		bcode.push_back('\0'); // put in a null terminator for C functions
		auto p1 = bcode.size(); //the address to jump to
		auto p2 = p0 +1; // address where the string starts
		bcode[p0] = p1; // fill in the jump address
		auto len = p1-p2-1; // length of the string
		//cout << "strings:strlen:" << len << "\n";
		create_push(bcode, p2);
		create_push(bcode, len);
	}
	break;
```
The code breaks down as follows:
1. Send an uncoditional jump instruction, `j`, to the byte code. The purpose is that when the bcode is interpreted, it will perform a jump past the end of the string. We don't know where we have to jump to at this stage, so we omit a dummy address (`?`) to be resolved further down
2. We work our way through the string in the program, adding the string contents to the byte code. We also push back a null terminator for the convenience of C functions wanting to use the string
3. Now that we know where in the bcode we can jump to, we fill in this address at the space we reserved in step 1.
4. We arrange to have the address and length of the string pushed onto the stack at interpretations time via `create_push()`. 

Because we want to write the `p` (push) instruction in several places, we perform a small refactoring, and create the following function:
```
template<class T>
void  create_push(bytes& bcode, T val)
{
        pushchar(bcode, 'p');
        push64(bcode, val);
}
```

## Changes to the byte-code interpreter

We need to handle the instruction `>`:
```
case '>': // jump if positive
	{
		auto v = pop_stack();
		++pc;
		pc = v>0 ? bcode[pc] : pc+1;
	}
	break;
```
It's just a small variation on `<`. Instead of testing for `v<0` to decide how to alter the program counter, we test for `v>0`.

We need to be able to perform an unconditional jump:
```
case 'j': // unconditional jump
	pc = bcode[++pc];
	break;
```

## Testing

As our program can now be quite tricky, at 335 lines long, it would be nice to automate our testing, and be sure we do not introduce any regressions into the code. 

Assembly code is now stored in a directory called `asm`. We add a bash script, `run-tests`  to process the assembly:
```
#!/usr/bin/env bash

mkdir -p out verified
rm -f out/*.out

for in in $( ls ../asm/v[123]*); do
        #echo $in
        out=`basename -s asm $in`out
        ./bcode < $in >out/$out
        diff out/$out verified/$out
done
```

It processes assembly files in the asm directorym , and writes out the results to a directory called `out`. Im directory `verified`, we store the expected result for each program. The results in `out` are compared with the expected results in `verified` to ensure they match.


## Example programs

In the `asm` directory, we have created two programs to test out our new functionality. The simplest on is `v3-string.asm`:
```
# print a string

p042 xemit # print an asterisk

'hello world'
xprin

' from bcode'
xprin

p042 xemit # another asterisk

p010 xemit # newline

0
```
It will just print out the line:
```
*hello world from bcode*
```

The compiler and interpreter is flexed more widely in the program `v3-string5.asm`:
```
# print a string 5 times

p005
L1

'hello world' xprin
' from bcode' xprin
p010 xemit

xdecr xdupe >1 # possibly loop

0
```
This prints out `hello world from bcode` five times. 

## What's next?

There are a few directions in which `bcode` could be extended:
1. instead of restricting `x` functions to 4 characters, it could be extended to allow for an arbitrary length function
2. addressing could be extended. Instead of restricting the bcode to be 256 bytes long, the length of the addressing word could be generalised. This would require careful testing (hurrah for automated testing!), as there are a number of places where the placement of addresses and the manipulation of the program counter are handled
3. creation of an interpreted language. Something like BASIC, for example the language suggested by Bennett, would make an ideal testing ground. Stack frames and recursive functions need to be handled.


