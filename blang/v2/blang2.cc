#include <iostream>
#include <sstream>
#include <string>

using std::cout;
using std::string;

const string gmr = R"(
(foo (bar baz))
)";

//const string gmr = "";

std::stringstream ss(gmr);

int getch()
{
	return ss.get();
}

void parse_list()
{
	// TODO
}

void parse_symbol()
{
	// TODO
}

void reader()
{
	/*
	while(char c = getch()  ) {
		if(ss.eof()) break;
		cout << c;
	}
	*/
	
	switch(int c = getch()){
		case '\n' :
		case '\t':
		case ' ':
		case '\r':
			break;
		case '(':
			parse_list();
		default:
			parse_symbol();
	}
	

}

int main()
{
	reader();
}
