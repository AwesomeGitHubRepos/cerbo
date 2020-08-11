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

typedef uint64_t cell_t;

vector<cell_t> bcodes;


enum ttype_t { QSTR, INT, ID, UNK };
typedef struct { ttype_t t; string lexeme; } token_t;
typedef vector<token_t> tokens_t;
tokens_t tokens;


ifstream ifs;




void push_toke(ttype_t t, string toke) { tokens.push_back( {t, toke} ); }

void tokenise()
{
	char c;
	while(ifs.get(c)) {
		string toke;
		if(c == '\'') {
			while(ifs.get(c) && c != '\n');
		} else if (c == '"') {
			while(ifs.get(c) && c != '"') toke += c;
			push_toke(QSTR, toke);
		} else if (isspace(c)) {
			// do nothing, just eat it up
		} else if (isdigit(c)) {
			toke = c;
			while(ifs.get(c) && isdigit(c)) toke += c;
			ifs.unget();
			push_toke(INT, toke);
		} else if (isalpha(c)) {
			toke = c;
			while(ifs.get(c) && isalpha(c)) toke += c;
			ifs.unget();
			push_toke(ID, toke);
		} else {
			push_toke(UNK, string{c});
		}
	}
}

void print_tokens() {
	cout << "Printing tokens\n";
	for(auto& t: tokens) {
		cout << t.t << "\t" << t.lexeme << "\n";
	}
}

int main(int argc, char** argv)
{
	if(argc != 2) {
		puts("Wrong number of arguments passed in");
		return 1;
	}

	string progfile = argv[1];
	cout << "Parsing file: " << progfile << endl;

	ifs.open(progfile);
	tokenise();
	ifs.close();

	print_tokens();

	return 0;
}
