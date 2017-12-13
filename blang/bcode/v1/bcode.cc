#include <cassert>
#include <fstream>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

typedef vector<uint8_t> bytes;

///////////////////////////////////////////////////////////////////////
// the stack

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

///////////////////////////////////////////////////////////////////////
// An extensible collection of functions.
// The idea is that you write more functions here that are relevant
// to your application.

void emit()
{
	int c = pop_stack();
	putchar(c);
}

void hello()
{
	puts("hello world");
}


typedef struct { 
	string  name; 
	function<void()> fn; 
} func_desc;


vector<func_desc> vecfns = {
	{"emit", emit},
	{"hell", hello}
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
// Convenience and utility functions


	
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
	cout << "push64 function pointer: " << int_to_hex(v64) << "\n";
	for(int i = 0; i<8 ; ++i)
		bs.push_back(b[i]);
}


int main()
{

	// read program
	stringstream sstr;
	sstr << cin.rdbuf();
	string prog = sstr.str();

	
	// compile
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

	ofstream bin;
	bin.open("bin.out");
	for(auto b:bcode) bin << b;
	bin.close();
	cout << "wrote bin.out\n";


	// now run the byte code
	int pc = 0;
	bool running = true;
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
	
	cout << "bcode halted\n";

	return 0;

}
