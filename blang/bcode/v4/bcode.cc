#include <cassert>
//#include <deque>
#include <fstream>
#include <cstddef>
#include <functional>
//#include <iomanip>
#include <iostream>
#include <map>
//#include <sstream>
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

void nb(const string& msg) { cerr << msg << endl; }


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
}

void push_bcode(int opcode, const value_t& v)
{
	encode(IP++, opcode, v);;
}
void push_bcode(int opcode) { push_bcode(opcode, monostate()); }

enum { QSTR=1, INT, ID, UNK, EOI, END, GOTO, PRINT, PUSH, OP, NEG, ASS, IF };
map<int, string> typemap = { 
	{QSTR, "QSTR"}, {INT, "INT"}, {PRINT, "PRINT"}, {PUSH, "PUSH"}, {ID, "ID"}, 
	{UNK, "UNK"}, {EOI, "EOI"}, {END, "END"}, {GOTO, "GOTO"}, {OP, "OP"}, 
	{NEG, "NEG"}, {ASS, "ASS"}, {IF, "IF"}  };

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

map<string, value_t> vars;

//void yyerror(const string& msg);

void panic() { throw 13; }

void do_op(char optype)
{
	int v1 = get<int>(pop()), v2 = get<int>(pop());
	int res;
	switch(optype) {
		case '+':	res = v2+v1; break;
		case '-':	res = v2-v1; break;
		case '*':	res = v2*v1; break;
		case '/':	res = v2/v1; break;
		default:	cerr << "do_op: unrecognised operator\n"; panic();
	}
	push(res);
}

void eval ()
{
	cout << "Running evaluator" << endl;
	IP = 0;
	while(1) {
		auto [opcode, value] = decode(IP++);
		switch (opcode) {
			case ASS:	vars[str(value)] = pop();		break;
			case ID:	push(vars[str(value)]);			break;
			case IF:	if(get<int>(pop()) ==0) IP = get<int>(value); break;
			case GOTO:	IP = get<int>(value); 			break;
			case NEG:	push(-get<int>(pop()));			break;
			case OP: 	do_op(str(value)[0]); 			break;
			case PRINT: 	cout << str(pop()) << "\n"; 		break;
			case PUSH: 	push(value); 				break;
			case END: 	cout << "STOP\n"; goto finis; 		break;
			default:
					cerr << "EVAL: opcode unknown: " << opcode << "\n";
					cerr << "Possible type: " <<typemap[opcode] << "\n";
					cerr << "IP: " << IP << "\n";
					exit(1);
		}
	}
finis:
	cout << "Exiting eval\n";
}

stack<int> holes; // jump statements
void dig_hole () { holes.push(IP-1);}

void fill_hole() 
{ 
	//int where = holes.top(); 
	opvalues[holes.top()] = IP;
	holes.pop(); 

	//return v; 
}

#define ttype ttypes[TIDX]
#define ttype_1 ttypes[TIDX+1]
#define tval tvals[TIDX]
#define tval_1 tvals[TIDX+1]

void yyerror(const string& msg)
{	
	cerr << "yyparse() failure: " << msg << "\n";
	cerr << "\tTIDX: " << TIDX << "\n";
	cerr << "\ttoken type: " << ttype << " ";
	auto f = typemap.find(ttype);
	if(f == typemap.end())
		cout << "???";
	else
		cout << f->second;
	cerr << "\n";
	cerr << "\ttoken value: " << tval << "\n";
	throw 3617;
	exit(1);
}

void require(const string& str)
{
	//TIDX++;
	if(str==tval) return;
	yyerror("Expected " + str + ", found " + tval);
}

void parse_expr_t ();

void parse_expr_p ()
{
	TIDX++;
	//cout << "parse_expr_p:" << tval << "\n";

	// optional negation
	//bool neg = false;
	if(ttype == OP and tval == "-") {
		parse_expr_p();
		push_bcode(NEG);
		return;
	}

	if(ttype == INT) {
		push_bcode(PUSH, stoi(tval));
	} else if(ttype == QSTR) {
		push_bcode(PUSH, tval);
	} else if(tval == "(") {
		parse_expr_t();
		TIDX++;
		require(")");
	} else if(ttype=ID) {
		push_bcode(ID, tval);
	} else {
		yyerror("parse_expr_p expected int");
	}

}

void parse_expr_t ()
{
	parse_expr_p();
	while(ttype_1 == OP) {
		auto op = tval_1;
		TIDX++;
		parse_expr_p();
		push_bcode(OP, op);
	}
}



	template<class K, class V>
V _find_or_die(const char* fname, int linenum, K key, map<K, V>  m, const string& msg)
{
	auto f = m.find(key);
	if(f != m.end()) return f->second;

	cout << "FATAL: Key not found:" << fname << ":" << linenum << ":" << msg << "\n";
	exit(1);
}
#define find_or_die(k, m, msg) _find_or_die(__func__, __LINE__, k, m, msg)




bool more = true;

void parse_stm();

void parse_end() { push_bcode(END, ""); }

void parse_eoi() { more = false; push_bcode(EOI, ""); }

void parse_id()
{
	string id = tval;
	TIDX++;
	if(tvals[TIDX] == ":") {
		goto_here[id] = IP;
	} else { // assignment
		//TIDX--;
		string varname = id;
		require("=");
		parse_expr_t();
		push_bcode(ASS, varname);
		//TIDX++;
	}
}



void parse_goto()
{
	if(ttypes[++TIDX]==ID) {
		push_bcode(GOTO, 666); // will need backfilling
		add_goto(tvals[TIDX]);
	} else {
		yyerror("GOTO expected an ID");
	}
}


void parse_print() { parse_expr_t(); push_bcode(PRINT, monostate()); }

void parse_if()
{
	//TIDX--;
	//nb("next instruction 1: " + tval);
	parse_expr_t();
	//nb(str(TIDX));
	push_bcode(IF, -1);
	dig_hole();
	TIDX++;
	//nb("next instruction 2: " + tval);
	parse_stm();
	TIDX--;
	fill_hole();
}

void parse_stm ()
{
	using vfunc = function<void()>;
	static const map<int, vfunc> stm_map =  {
		{EOI, 	parse_eoi},
		{END, 	parse_end},
		{GOTO, 	parse_goto},
		{ID, 	parse_id},
		{IF, 	parse_if},
		{PRINT, parse_print}
	};

	auto fn = find_or_die(ttype, stm_map, "Unrecognised statement type:" + str(ttype) 
			+ ":TIDX:" + str(TIDX) + ":tval:" + str(tval));
	fn();
	TIDX++;
}

void yyparse()
{
	opcodes.reserve(10000);
	opvalues.reserve(10000);
	more = true;
	TIDX=0;

	while(more) parse_stm();
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
		} else if(c=='+' || c=='-' || c=='*' || c == '/') {
			push_toke(OP, string{c});
		} else {
			push_toke(UNK, string{c});
		}
	}
	push_toke(END, "END"s);
	nb("about to push EOI");
	push_toke(EOI, "EOI"s); // cap it all off with an End Of Input
	nb("pushed");
}

void print_tokens() {
	cout << "Printing tokens\n";
	int i = 0;
	while(ttypes[i] != EOI) {
		cout << "\t" << i << "\t" << typemap[ttypes[i]] << "\t" << tvals[i] << "\n";
		//cout << "Done\n";
		i++;
	}
	cout << "Finished printing tokens\n";
}

void print_bcodes()
{
	cout << "Printing bcodes" << endl;
	int i=0;
loop:
	auto[opcode, value] = decode(i);
	cout << "\t" << i << "\t" << typemap[opcode] << "\t" << str(value) << endl;
	i++;
	if(opcode != EOI) goto loop;

	cout << "Finished printing bcodes" << endl;

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


void check_stack()
{
	if(dstack.empty()) return;
	cerr << "ERR: Exiting with non-empty stack.\n";
	cerr << "Stack contents, top downwards: ";
	while(! dstack.empty()) {
		cerr << str(dstack.top()) << " ";
		dstack.pop();
	}
	cerr << "\n";
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

	check_stack();

	return 0;
}
