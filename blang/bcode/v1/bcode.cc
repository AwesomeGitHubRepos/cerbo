#include <cassert>
#include <fstream>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stack>
#include <string>
#include <string.h>
#include <vector>

using namespace std;

typedef vector<uint8_t> bytes;
typedef function<void()> vptr;

stack<int64_t> stk; // the stack

template< typename T >
std::string int_to_hex( T i )
{
	std::stringstream stream;
	stream << "0x" 
		<< setfill ('0') << setw(sizeof(T)*2) 
		<< hex << i;
	return stream.str();
}

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

void emit()
{
	int c = pop_stack();
	putchar(c);
}

void hello()
{
	puts("hello world");
}

map<string, vptr>  functions = {
	{"emit", emit},
	{"hell", hello}
};


typedef struct { string  name; function<void()> fn; } func_desc;
vector<func_desc> vecfns = {
	{"emit", emit},
	{"hell", hello}
};


vptr find_function(string name)
{
	auto it = functions.find(name);
	if(it != functions.end())
		return it->second;
	else
		return nullptr;

}

int find_vecfn(string name)
{
	for(int i = 0 ; i<vecfns.size(); ++i)
		if(vecfns[i].name == name)
			return i;

	cerr << "find_vecfn:unknown function:" << name << "\n";
	exit(1);
}

void test3()
{
	cout <<"test3\n";
	auto fn = vecfns[find_vecfn("hell")].fn;
	fn();
}

int main()
{

	cout << "Test1: Should print a !\n";
	push_stack(33); // ASCII !
	emit(); // works
	cout << "\nTest1 finished\n"; 

	cout << "Test 2: s/b same result\n"; // works
	{
		push_stack(33);
		auto fn = find_function("emit");
		fn();
		cout << "emit:" << &fn << "\n";
	}
	cout << "\nTest 2 finisned\n";

	test3();
	//exit(0);

	string prog("p072 xemit p073 xemit p010 xemit xhell 0");
	
	//prog = "xhell 0";

	// compile
	bytes bcode;
	for(int i = 0 ; i < prog.size(); ++i) {
		char c = prog[i];
		switch(c) {
			case ' ': break; // ignore spaces
			case '0' : 
				  pushchar(bcode, '0'); 
				  break;
			case 'p' :{
       					  pushchar(bcode, 'p');
					  auto val = (prog[++i] -'0') * 100 + (prog[++i]-'0')*10 +(prog[ ++i]- '0');
					  cout << "compiling p:" << val << "\n";
       					  push64(bcode, val);
				  }
				  break;				   
			case 'x': {
       					  pushchar(bcode, 'x');
       					  string function_name = { prog[++i],  prog[++i], prog[++i], prog[++i]};
					  pushchar(bcode, find_vecfn(function_name)); 
					  /*
					  cout << "function name:" << function_name << ".\n";
					  auto function_ptr = find_function(function_name);
					  int64_t ptr = reinterpret_cast<int64_t>(&function_ptr);
					  cout << "x function pointer: " << int_to_hex(ptr) << "\n";
					  if(function_ptr)
						  push64(bcode, ptr);
					  else {
						  cerr << "Could not find function:" << function_name << ".\n";
						  exit(1);
					  }*/
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
					  /*
					  uint8_t b[8];
					  int64_t addr = 0 ; // (int64_t) b;
					  for(uint8_t i = 0 ; i <8; ++i) b[i] = bcode[++pc];
					  memcpy(&addr, b, 8);
					  cout << "x function pointer: " << int_to_hex(addr) << "\n";
					  reinterpret_cast< void(*)() > (addr) ();
					  */
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
