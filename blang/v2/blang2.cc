#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

using std::cout;
using std::string;

constexpr bool iswhite(char c) { return c == '\n' || c=='\t' || c==' ' || c=='\r';}

const string gmr = R"(
(foo (bar baz))
)";

//const string gmr = "";

enum cell_e { T_NUL, T_STR, T_DBL };

struct cell_t {
	cell_e type;
	void *ptr = nullptr;
} cell_t;

//class cell;

class cell {
	public:
		//cell() {}
		//cell(string s): value{s} {}
		typedef std::vector<cell> cellvec;
		std::variant<double, std::string, cellvec> value;
};
/*
typedef std::variant<std::string, double> pre_cell;
typedef std::vector<pre_cell> cell_list;
typedef std::variant<pre_cell, cell_list> cell;
*/

std::map<std::string, cell> vars;

std::stringstream ss;

int peek() { return ss.peek(); }

int getch() { return ss.get(); }

cell read();

cell::cellvec parse_list()
{
	cell::cellvec res;
	while(char c = getch()) {
		if(c==')') 
			break;
		else {
			res.push_back(read());
		}
	}


	cout << "parse_list(): TODO\n";
	return res;
}

std::string
parse_symbol()
{
	string str;
	int c;
	while(c=getch()){
		if(iswhite(c)) break;
		if(c == '(' || c == ')') break;
		str +=c;
	}
	return str;
}

void
eval(std::string s)
{
	cout << "symbol is<" << s << ">\n";
	auto it = vars.find(s);
	if(it == vars.end()) {
			cout << ("Ouch: couldn't find value for variable `" + s + "'\n");
       	} else {
			cout << "TODO eval string\n";
	}

}

cell
read()
{
	/*
	while(char c = getch()  ) {
		if(ss.eof()) break;
		cout << c;
	}
	*/

	cell res; // = "TODO";
	while(char c = getch()) {
		if(ss.eof()) break;
		//cout << "c=<" << c << ">\n";
		if(c=='(') 
			parse_list();
		else if(!iswhite(c)){
			ss.unget();
			string s = parse_symbol();
			//res.value.copy<std::string>(parse_symbol());
		}

	}

	return res;

}

void run_tests();

int main()
{

	run_tests();
	//reader();
}


void run_tests()
{
	ss << "(define foo 12) foo   ";
	
	read();
}
