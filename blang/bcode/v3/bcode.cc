#include <cassert>
#include <deque>
#include <fstream>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
//#include <stack>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

typedef vector<uint8_t> bytes;
bytes bcode; 

///////////////////////////////////////////////////////////////////////
// address labels


// The addresses of labels you create via the L command
uint8_t labels[256];

// what addresses refer to those labels
typedef struct { 
	uint8_t label_name; 
	uint8_t position;
} lref_t;

vector<lref_t> label_refs;

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

///////////////////////////////////////////////////////////////////////
// the stack

deque<int64_t> stk; // Use a deque instead of a stack, because we might want to print it

int64_t pop_stack()
{
	int64_t v = stk.back();
	stk.pop_back();
	return v;
}

void push_stack(int64_t v)
{
	stk.push_back(v);
}

///////////////////////////////////////////////////////////////////////
// Convenience and utility functions


void print_stack()
{
	cout << "Stack contents:";
	for(auto it = stk.begin(); it != stk.end(); ++it)
		cout << " " << *it;
	cout << "\n";
}
	
template< typename T >
std::string int_to_hex( T i )
{
	std::stringstream stream;
	stream << "0x" 
		<< setfill ('0') << setw(sizeof(T)*2) 
		<< hex << i;
	return stream.str();
}

///////////////////////////////////////////////////////////////////////
// An extensible collection of functions.
// The idea is that you write more functions here that are relevant
// to your application.


void decr()
{
	//print_stack();
	stk.back() -= 1;
}

void dup()
{
	push_stack(stk.back());

}

void emit()
{
	int c = pop_stack();
	putchar(c);
}

void hello()
{
	puts("hello world");
}

void incr()
{
	stk.back() += 1;
}

void print_string()
{
	//cout << "print_string: stack before:\n"; print_stack();
	auto len = pop_stack();
	auto pos = pop_stack();
	//cout << "print_string:len:" << len << "\n";
	//cout << "print_string:pos:" << pos << "\n";
	for(auto i=0; i< len; ++i)
		cout << bcode[pos+i];
	//cout << "print_string: stack after:\n"; print_stack();
}

void subt() // a b -- a-b
{
	int64_t tmp = pop_stack();
	stk.back() -= tmp;

}


typedef struct { 
	string  name; 
	function<void()> fn; 
} func_desc;


vector<func_desc> vecfns = {
	{"decr", decr},
	{"dupe", dup},
	{"emit", emit},
	{"hell", hello},
	{"incr", incr},
	{"prin", print_string},
	{"subt", subt}
};



int find_vecfn(string name)
{
	for(int i = 0 ; i<vecfns.size(); ++i)
		if(vecfns[i].name == name)
			return i;

	cerr << "find_vecfn:unknown function:" << name << "\n";
	exit(1);
}


///////////////////////////////////////////////////////////////////////

void pushchar(bytes& bs, char c)
{
	bs.push_back(c);
}

template<class T>
void push64(bytes& bs, T  v)
{
	int64_t v64 = v;
	uint8_t b[8];
	*b = v64;
	//cout << "push64 function pointer: " << int_to_hex(v64) << "\n";
	for(int i = 0; i<8 ; ++i)
		bs.push_back(b[i]);
}


template<class T>
void  create_push(bytes& bcode, T val)
{
	pushchar(bcode, 'p');
	push64(bcode, val);
}

int main()
{

	// read program
	stringstream sstr;
	sstr << cin.rdbuf();
	string prog = sstr.str();

	
	// compile
	//bytes bcode;
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
			case '0' : 
				  pushchar(bcode, '0'); 
				  break;
			case '<':
			case '>':
				  pushchar(bcode, c);
				  label_refs.push_back({prog[++i], (uint8_t) bcode.size()});
				  pushchar(bcode, '?'); // placeholder for an address to be resolved later
				  break;
			case 'L':
				  labels[prog[++i]] = bcode.size();
				  break;
			case 'p' :{
					  auto val = (prog[++i] -'0') * 100 + (prog[++i]-'0')*10 +(prog[ ++i]- '0');
					  create_push(bcode, val);
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
	resolve_labels(bcode);

	ofstream bin;
	bin.open("bin.out");
	for(auto b:bcode) bin << b;
	bin.close();
	cout << "wrote bin.out\n";

	//exit(1);

	// now run the byte code
	int pc = 0;
	bool running = true;
	while(running) {
		uint8_t b = bcode[pc];
		switch(b) {
			case '0':
				running = false;
				break;				
			case '<': // jump if negative
				{
					auto v = pop_stack();
					++pc;
					pc = v<0 ? bcode[pc] : pc+1;
					/*
					if(v<0) 
						pc = bcode[pc];
					else
						++pc;
						*/
				}
				break;
			case '>': // jump if positive
				{
					auto v = pop_stack();
					++pc;
					pc = v>0 ? bcode[pc] : pc+1;
					//cout << "Setting pc to" << pc << "\n";
				}
				break;
			case 'j': // unconditional jump
				pc = bcode[++pc];
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
	cout << "bcode halted\n";

	print_stack();

	return 0;

}
