#include <iostream>
#include <string>
#include <tuple>
using namespace std::string_literals;

using namespace std;

const char LRB = '(', RRB = ')';

int ch;
int yylval;;
string yytext;


void trace(string s)
{
	cout << "trace:" << s << "." << endl;
}

int nextch() { ch = getchar(); return ch; }

void eat_white() { while(isblank(ch)) nextch(); }

// will have form offset ...?
void offsetx(char& rx, char&ry, int& offset)
{
	while(isdigit(ch)) { offset = offset*10 + (ch - '0') ; nextch(); }
	if(ch != LRB) return;
	nextch(); // 'R'
	rx = nextch();
	nextch(); // RRB
	nextch(); // ,
	//trace("foo");
	//trace("ox"s + (char)ch);
	if(ch != ',') return;
	nextch(); // LRB
	nextch(); // 'R'
	ry = nextch();
}

void rxry(char& rx, char&ry, int& offset)
{
	rx = nextch();
	nextch(); // ,
	while(isdigit(ch)) { offset = offset*10 + (ch - '0') ; nextch(); }
	//if(ch == ',') nextch();
	if(ch == LRB) nextch();
	if(ch == 'R') nextch();
	ry = ch;
}

void instruction()
{
	trace("instruction");
	eat_white();
	string code;
	while(!isspace(ch)) { code += ch; nextch(); }
	trace("code:" + code);
	eat_white();

	int offset =0;
	char rx='0', ry='0';

	if(isdigit(ch))
		offsetx(rx, ry, offset);
	else if(ch == 'R')
		rxry(rx, ry, offset);

	trace("ins:rx:"s + rx  + ",ry:"  + ry + ",off:" + to_string(offset));
	//cout << rx << ry << offset << endl;


}

void parse_line()
{
	if(isblank(ch)) return instruction();

}

int main()
{
	/*
	string line;
	while(!getline(cin, line)) {
		cout << line << "\n";
	}
	*/
	while((ch = getchar()) != EOF)
		parse_line();

	return 0;
}
