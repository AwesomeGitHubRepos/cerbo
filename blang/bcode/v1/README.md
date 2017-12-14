# Part 1 - just get something working

Our assembly language will be particularly brittle and simple. I just want to see something working.


## Instruction set

Let's define our instruction set for the assembly language. We won't worry about jumps and conditionals yet. We just want to get something working.

| Instruction | Meaning                   |
| ----------- | ------------------------- |
| #           | Comment                   |
| 0           | halt execution            |
| p'nnn'      | push <nnn> onto stack     |
| x'cccc'     | execute instruction <nnn> |

Notes: 
* Comments begin with `#` and last until the end of the line.
* white spaces are ignored *between* instructions
* `0' stops the interpreter
* `n` represents a digit `0..9`. p'nnn' means the letter p followed by exactly 3 digits. e.g. `p321`.
* `c` represents any ASCII character. x'cccc' means x followed by 4 chars.

The stack takes 64-bit words, and you push a number onto the stack using the `p` command.

`x` means "execute a vectored instruction". The idea is that you extend the instruction set of the interpreter, and associate a 4-letter mnemonic with it. I have provided two canned instructions for illustrative purposes:
* `xhell` - which simply prints the phrase "hello world" to output
* `xemit` - which pops a value from the top of the stack, and prints it.

## Examples

The program `hello.asm` prints "hello world":
```
# simplest program possible
# just print 'hello world'
xhell
0
```

Note that it is not necessary to have white spaces between instructions. I could have simple written `xhell0` if I chose. 

Let's use the full extend of the instruction set to print "HI" to stdout. The program `hi.asm` does this:
```
# print HI to screen
p072  # ascii H
xemit # print the char to screen
p073  # ascii I
xemit
p010  # \n
xemit
0     # halt program
```

It is failry well documented. iUsing the instruction `p072`, we push the number 72 to the stack, which is the ascii value for "H". We then call `xemit`, which prints it to the console. We do likewise for "I", which has a decimal value 73, and for the carriage return, which has a value of 10. Finally, we call `0` to halt the interpreter.

## Discussion of the code.

The code is in file `bcode.cc`, and can be compiled using `make`. Programs can be run by passing them in stdin, e.g. `./bcode < hi.asm`.

We define the machine instructions as a sequence of bytes:
```
typedef vector<uint8_t> bytes;
```
We will tag bytes onto the end of the instructions as we go.


We set up the virtual machine's stack, providing `pop` and `push`:
```
stack<int64_t> stk;

int64_t pop_stack()
{
        int64_t v = stk.top();
        stk.pop();
        return v;
}

void push_stack(int64_t v)
{
        stk.push(v);
}
```

Let's define the functions `emit` and `hell` that we described above:
```
void emit()
{
        int c = pop_stack();
        putchar(c);
}

void hello()
{
        puts("hello world");
}
```

We need to make these functions visible to our intepreter:
```
typedef struct { 
        string  name; 
        function<void()> fn; 
} func_desc;


vector<func_desc> vecfns = {
        {"emit", emit},
        {"hell", hello}
};
```

and we need to find a way of finding the function given its interpreter name:
```
int find_vecfn(string name)
{
        for(int i = 0 ; i<vecfns.size(); ++i)
                if(vecfns[i].name == name)
                        return i;

        cerr << "find_vecfn:unknown function:" << name << "\n";
        exit(1);
}
```
We define a function for push a char onto the instruction set:
```
void pushchar(bytes& bs, char c)
{
        bs.push_back(c);
}
```
and similarly, to push a 64-bit word:
```
template<class T>
void push64(bytes& bs, T  v)
{
        int64_t v64 = v;
        uint8_t b[8];
        *b = v64;
        cout << "push64 function pointer: " << int_to_hex(v64) << "\n";
        for(int i = 0; i<8 ; ++i)
                bs.push_back(b[i]);
}
```

The bulk of the processing is done in `main()`. We read the input stream into a string called `prog`.

We then process that string to compile into a sequence of bytecodes:
```
       bytes bcode;
        for(int i = 0 ; i < prog.size(); ++i) {
                char c = prog[i];
                switch(c) {
                        case ' ': // ignore white space
                        case '\r':
                        case '\t':
                        case '\n':
                                break; 
                        case '#': // ignore comments
                                while(prog[++i] != '\n');
                                break;
                        case '0' : 
                                  pushchar(bcode, '0'); 
                                  break;
                        case 'p' :{
                                          pushchar(bcode, 'p');
                                          auto val = (prog[++i] -'0') * 100 + (prog[++i]-'0')*10 +(prog[ ++i]- '0');
                                          //cout << "compiling p:" << val << "\n";
                                          push64(bcode, val);
                                  }
                                  break;                                   
                        case 'x': {
                                          pushchar(bcode, 'x');
                                          string function_name = { prog[++i],  prog[++i], prog[++i], prog[++i]};
                                          pushchar(bcode, find_vecfn(function_name)); 
                                          break;
                                  }
                        default:
                                   cerr << "Compile error: Unknown code at position " << i << ":" << c << "\n";
                                   exit(1);
                }

        }
```
Here, `i` is an index on the input program characters. You can see that white spaces and comments are just ignored. The simplest instruction is `0`, which just pushes the literal `0` onto the byte-code.

The next instruction is `p`, which takes the next three numbers from the program, converts them into an integer, and pushes this value as byte-codes.

The last instruction is `x`, It takes the next four characters as a function name, and finds the index number in our list of executable instructions. It them pushes this number to the byte-codes. Note that only one byte is pushed, so we are only allowed a maximum of 256 functions. This should be plenty sufficient for our purposes here. 

That's it! A byte-code compiler in approximately 30 lines of code. 

So, we have `bcode`, which is our sequence of instructions. We can put them out into a file `bin.out`. We can then execute the byte-code.

First, we point our program counter, `pc`, which points to the current instruction in `bcode`, to 0:
```
	int pc = 0;
```

We set the interpreter running:
```
        bool running = true;
```
and we keep running until we encounter `0`, at which point we know to stop the interpreter. So the interpretation loop looks like this:
```
       while(running) {
                uint8_t b = bcode[pc];
                switch(b) {
                        case '0':
                                running = false;
                                break;
                        case 'p': {
                                          uint8_t b[8];
                                          for(int i = 0 ; i <8; ++i) b[i] = bcode[++pc];
                                          int64_t v64 = *b;
                                          push_stack(v64);
                                          pc++;
                                  }
                                break;
                        case 'x': {
                                          auto fn_idx = bcode[++pc];
                                          auto fn = vecfns[fn_idx].fn;
                                          fn();
                                          pc++;

                                  }
                                break;
                        default:
                                cerr << "Illegal instruction at PC " << pc << ":" << b << "\n";
                                exit(1);
                }
        }
```
As you can see, `0` just sets `running = false` to halt execution.

`p` takes the next 8 bytes (a 64-bit word) from `bcode`, and pushes it onto the stack. The program counter is updates accordingly.

`x` is perhaps a bit more interesting. It looks up the next byte from the instruction set, which is interpreted as an index for the function. It then retrieves the function pointer from the index:
```
                                          auto fn = vecfns[fn_idx].fn;
```
and calls it:
```
                                          fn();
```
Don't forget that we must also advance the program counter:
```
                                          pc++;
```

As with the compiler, the interpreter only takes about 30 lines of code. 

I have not yet enabled jumping, which is vital to a really useful intetpreter. I have also ommitted the embedding of strings, which is also very handy.

Until next time, though.
