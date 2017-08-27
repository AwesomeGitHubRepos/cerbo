#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

using std::cout;
using std::string;


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

typedef struct Nothing {} Nothing; // not anything

class cell {
	public:
		//cell() {}
		//cell(string s): value{s} {}
		typedef std::vector<cell> cellvec;
		std::variant<Nothing, std::string, cellvec, double> value;
};
/*
typedef std::variant<std::string, double> pre_cell;
typedef std::vector<pre_cell> cell_list;
typedef std::variant<pre_cell, cell_list> cell;
*/

std::map<std::string, cell> vars;

std::stringstream ss;
constexpr bool iswhite(char c) { return c == '\n' || c=='\t' || c==' ' || c=='\r';}
bool issym(char c) { return !(iswhite(c) || '(' || ')' || ss.eof()); }

int peek() { return ss.peek(); }

int getch() { return ss.get(); }

void
eat_white()
{
	while(iswhite(peek())) 
		getch();
}

cell read();

cell::cellvec parse_list()
{
	cout << "parse_list()...\n";

	cell::cellvec cells;
	while(char c = getch()) {
		//cout << c ;
		if(ss.eof())
			throw std::runtime_error("parse_list(): unmatched parenthesis");
		if(c==')') 
			break;
		else {
			ss.unget();
			cells.push_back(read());
		}
	}

	//cell res;
	//res.value = cells;
	//cout << "... parse_list(): index= " << res.value.index() << "\n";
	return cells;
}

std::string
parse_symbol()
{
	string str;
	int c;
	while(c=getch()){
		if(ss.eof()) break;
		if(iswhite(c)) break;
		if(c == '(' || c == ')') {ss.unget(); break;}
		str +=c;
	}

	cout << "parse_symbol(): <" + str + ">\n";
	return str;
}

std::string
eval_string(const std::string& s)
{
	cout << "symbol is<" << s << ">\n";
	auto it = vars.find(s);
	if(it == vars.end()) {
			cout << ("Ouch: couldn't find value for variable `" + s + "'\n");
       	} else {
			cout << "TODO eval string\n";
	}

	return s; // TODO

}
void
eval(const cell& c)
{
	//cout << "type is " << c.value.index() << "\n";

	if(std::holds_alternative<std::string>(c.value)) {
		cout << "eval holds string\n";
		std::string s = std::get<std::string>(c.value);
		cout << eval_string(s) << "\n";
	} else if(std::holds_alternative<double>(c.value)) {
		cout << "eval holds double\n";
	} else if(std::holds_alternative<cell::cellvec>(c.value)) {
		cout << "eval holds cellvec\n";
	} else {
		cout << "eval holds unhandled\n";
	}
	cout << "eval TODO\n";

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
	eat_white();
	char c = getch();
       	if(ss.eof()) {
		// nothing
	} else if(c=='(') 
		res.value = parse_list();
	else {
		ss.unget();
		string s = parse_symbol();
		res.value = s;
		//res.value.copy<std::string>(parse_symbol());
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
	ss << "(define (foo  bar) 12) foo   ";
	

	while(!ss.eof()){
		cout << "=========================\n";
		cell c = read();
		eval(c);
	}
}
