#include <cassert>
#include <deque>
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
//#include <type_traits>
#include <variant>
#include <vector>


using namespace std;

typedef uint64_t cell_t;

typedef variant<monostate, int, string> value_t;
vector<int> opcodes;
vector<value_t> opvalues;

//template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
//template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
string str(const value_t& v)
{
	if(holds_alternative<monostate>(v)) return "";
	if(holds_alternative<int>(v)) return to_string(get<int>(v));
	if(holds_alternative<string>(v)) return get<string>(v);
	throw 788;
}

//vector<cell_t> bcodes;
cell_t IP = 0; 

void encode(int pos, int opcode, const value_t& value)
{
	opcodes[pos] = opcode;
	opvalues[pos] = value;
}

tuple<int, value_t> decode(int x)
{
	return {opcodes[x], opvalues[x]};
	//cell_t opcode = x >>56;
	//cell_t value = x- (opcode<<56);
	//return {opcode, value};
}

void push_bcode(int opcode, const value_t& v)
{
	encode(IP++, opcode, v);;
}


enum { QSTR=1, INT, PRIN, ID, UNK, EOI, END, GOTO, PRINT, PUSH };
map<int, string> typemap = { 
	{QSTR, "QSTR"}, {INT, "INT"}, {PRIN, "PRIN"}, {PRINT, "PRINT"}, {PUSH, "PUSH"}, {ID, "ID"}, 
	{UNK, "UNK"}, {EOI, "EOI"}, {END, "END"}, {GOTO, "GOTO"}  };

int TIDX = 0; // token index
vector<int> ttypes;
vector<string> tvals; // token values

map<string, int> goto_here;
vector<int> goto_ips;
vector<string> goto_labels;
void add_goto(const string& label)
{
	//cout << "add_goto:ref to:" << label << "\n";
	goto_ips.push_back(IP-1);
	goto_labels.push_back(label);
}

stack<value_t> dstack; // data stack
void push(const value_t& v) { dstack.push(v); }
value_t pop() { value_t v = dstack.top(); dstack.pop(); return v; }

void eval()
{
	cout << "Running evaluator\n";
	IP = 0;
	while(1) {
		auto [opcode, value] = decode(IP++);
		switch (opcode) {
			case GOTO:
				IP = get<int>(value);
				break;
			case PRIN:
				cout << str(value) << "\n";
				break;
			case PRINT:
				cout << str(pop()) << "\n";
				break;
			case PUSH:
				push(value);
				break;
			case END:
				cout << "STOP\n";
				goto finis;
				break;
			default:
				cerr << "EVAL: opcode unknown: " << opcode << "\n";
				cerr << "Possible type: " <<typemap[opcode] << "\n";
				exit(1);
		}
	}
finis:
	cout << "Exiting eval\n";
}

void yyerror(const string& msg)
{
	cerr << "yyparse() failure: " << msg << "\n";
	exit(1);
}

#define ttype ttypes[TIDX]
#define tval tvals[TIDX]

void parse_expr()
{
	TIDX++;
	if(ttype == INT)
		push_bcode(PUSH, stoi(tval));
	else
		yyerror("parse_expr: unexpected type found");
}

void parse_print()
{
	parse_expr();
	push_bcode(PRINT, 0);
}

void yyparse()
{
	opcodes.reserve(10000);
	opvalues.reserve(10000);

	//bcodes.reserve(10000); // plenty to keep us amused
loop:
	switch(ttypes[TIDX]) {
		case ID:
			{
				string id = tvals[TIDX];
				TIDX++;
				if(tvals[TIDX] == ":") {
					goto_here[id] = IP;
				}
				// TODO more here, like assignments
			}
			break;
		case GOTO:
			if(ttypes[++TIDX]==ID) {
				push_bcode(GOTO, 666); // will need backfilling
				add_goto(tvals[TIDX]);
			} else {
				yyerror("GOTO expected an ID");
			}
			break;
		case PRIN:
			//push_bcode(PRIN, stoi(tvals[++TIDX]));
			++TIDX;
			push_bcode(PRIN, tval);
			break;
		case PRINT:
			parse_print();
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

void push_toke(int t, const string& toke) { ttypes.push_back(t); tvals.push_back(toke); }

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
			//keyword_or_id(c);
			toke = c;
			while(ifs.get(c) && isalpha(c)) toke += c;
			ifs.unget();
			transform(toke.begin(), toke.end(), toke.begin(), ::toupper);

			for(auto& x: typemap) {
				if(x.second == toke) {
					push_toke(x.first, x.second);
					goto found_key;
				}
			}

			// not a keyword, so a regular ID
			push_toke(ID, toke);
found_key:
			continue;
		} else {
			push_toke(UNK, string{c});
		}
	}
	push_toke(END, "");
	push_toke(EOI, ""); // cap it all off with an End Of Input
}

void print_tokens() {
	cout << "Printing tokens\n";
	int i = 0;
	while(ttypes[i] != EOI) {
		cout << "\t" << i << "\t" << typemap[ttypes[i]] << "\t" << tvals[i] << "\n";
		i++;
	}
}

void print_bcodes()
{
	cout << "Printing bcodes\n";
	int i=0;
loop:
	auto[opcode, value] = decode(i);
	cout << "\t" << i << "\t" << typemap[opcode] << "\t" << str(value) << "\n";
	i++;
	if(opcode != EOI) goto loop;

	cout << "Finished printing bcodes\n";

}

void resolve_gotos()
{
	for(int i =0; i < goto_labels.size(); i++) {
		auto f = goto_here.find(goto_labels[i]);
		if(f == goto_here.end())
			yyerror("Can't resolve goto label:" + goto_labels[i]);
		auto dest = f->second;
		encode(goto_ips[i], GOTO, dest);

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

	yyparse();
	resolve_gotos();
	print_bcodes();

	eval();

	return 0;
}
