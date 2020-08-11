#include <cassert>
#include <deque>
#include <fstream>
#include <cstddef>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
//#include <stack>
#include <string>
#include <string.h>
#include <vector>


using namespace std;

typedef uint64_t cell_t;

vector<cell_t> bcodes;
cell_t IP = 0; 

cell_t encode(cell_t opcode, cell_t value) {	 return  (opcode << 56 ) + value; }
tuple<cell_t, cell_t> decode(cell_t x)
{
	cell_t opcode = x >>56;
	cell_t value = x- (opcode<<56);
	return {opcode, value};
}

void push_bcode(int opcode, int value)
{
	bcodes[IP++] = encode(opcode, value);;
}


enum ttype_t { QSTR, INT, PRIN, ID, UNK, EOI, END };
map<int, string> typemap = { {QSTR, "QSTR"}, {INT, "INT"}, {PRIN, "PRIN"}, {ID, "ID"}, {UNK, "UNK"}, {EOI, "EOI"}, {END, "END"} };
int TIDX = 0; // token index
vector<ttype_t> ttypes;
vector<string> tvals; // token values

void eval()
{
	cout << "Running evaluator\n";
	IP = 0;
	while(1) {
		auto [opcode, value] = decode(bcodes[IP]);
		switch (opcode) {
			case PRIN:
				cout << value << "\n";
				break;
			case END:
				cout << "STOP\n";
				goto finis;
				break;
			default:
				cerr << "EVAL: opcode unknown:" << opcode << "\n";
				exit(1);
		}
		IP++;
	}
finis:
	cout << "Exiting eval\n";
}

void yyparse()
{
	bcodes.reserve(10000); // plenty to keep us amused
loop:
	switch(ttypes[TIDX]) {
		case PRIN:
			push_bcode(PRIN, atoi(tvals[++TIDX].c_str()));
			break;
		case END:
			push_bcode(END, END);
			break;
		case EOI:
			push_bcode(EOI, EOI);
			return;
		default:
			cerr << "Unrecognised token type\n";
			exit(1);
	}
	TIDX++;
	goto loop;

}


ifstream ifs;

void push_toke(ttype_t t, const string& toke) { ttypes.push_back(t); tvals.push_back(toke); }

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
			transform(toke.begin(), toke.end(), toke.begin(), ::toupper);
			if(toke == "PRIN")
				push_toke(PRIN, toke);
			else
				push_toke(ID, toke);
		} else {
			push_toke(UNK, string{c});
		}
	}
	push_toke(END, "END");
	push_toke(EOI, "EOI"); // cap it all off with an End Of Input
}

void print_tokens() {
	cout << "Printing tokens\n";
	int i = 0;
	while(ttypes[i] != EOI) {
		cout << "\t" << typemap[ttypes[i]] << "\t" << tvals[i] << "\n";
		i++;
	}
}

void print_bcodes()
{
	cout << "Printing bcodes\n";
	int i=0;
loop:
	auto[opcode, value] = decode(bcodes[i++]);
	cout << "\t" << typemap[opcode] << "\t" << value << "\n";
	if(opcode != EOI) goto loop;

	cout << "Finished printing bcodes\n";

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

	yyparse();

	print_bcodes();

	eval();

	return 0;
}
