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


map<string, vptr>  functions = {
	{ "emit", emit}
};


vptr find_function(string name)
{
	auto it = functions.find(name);
	if(it != functions.end())
		return it->second;
	else
		return nullptr;

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

	//exit(0);

	string prog("p110 xemit p111 xemit p010 xemit 0");

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
					  cout << "function name:" << function_name << ".\n";
					  auto function_ptr = find_function(function_name);
					  int64_t ptr = reinterpret_cast<int64_t>(&function_ptr);
					  cout << "x function pointer: " << int_to_hex(ptr) << "\n";
					  if(function_ptr)
						  push64(bcode, ptr);
					  else {
						  cerr << "Could not find function:" << function_name << ".\n";
						  exit(1);
					  }
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
					  //= { *(++pc), *(++pc), *(++pc), *(++pc), ++pc, ++pc, ++pc, ++pc };
	  				  //push_stack(++pc + ++pc<<8 + ++pc<<16 + ++pc<<24 + ++pc<<32 + ++pc<<40 + ++pc<<48 + ++pc<<56);
					  int64_t v64 = *b;
					  push_stack(v64);
	  				  pc++;
				  }
				break;
			case 'x': {
	  				  void* addr = (void *) pop_stack();
					  reinterpret_cast< void(*)() > (addr) ();
					  pc++;
					  //using void (*void_f)(void);
					  //((void_f)addr)();

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
