#include <iostream>
#include <sstream>
#include <string>

using std::cout;
using std::string;

constexpr bool iswhite(char c) { return c == '\n' || c=='\t' || c==' ' || c=='\r';}

const string gmr = R"(
(foo (bar baz))
)";

//const string gmr = "";

std::stringstream ss;

int peek() { return ss.peek(); }

int getch()
{
	return ss.get();
}

void reader();

void parse_list()
{
	// TODO
}

void parse_symbol()
{
	cout << "parse_symbol() ...\n";
	string str;
	int c;
	while(c=getch()){
		if(iswhite(c)) break;
		if(c == '(' || c == ')') break;
		str +=c;
	}
	cout << "symbol is<" << str << ">\n";
}

void reader()
{
	/*
	while(char c = getch()  ) {
		if(ss.eof()) break;
		cout << c;
	}
	*/

	while(char c = getch()) {
		if(ss.eof()) break;
		//cout << "c=<" << c << ">\n";
		if(c=='(') 
			parse_list();
		else if(!iswhite(c)){
			ss.unget();
			parse_symbol();
		}

	}

}

void run_tests();

int main()
{

	run_tests();
	//reader();
}


void run_tests()
{
	ss << "foo bar baz   ";
	
	reader();
}
