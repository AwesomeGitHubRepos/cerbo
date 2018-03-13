/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org>
 */

#include <cassert>
//#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <regex>
#include <iterator>
#include <stdexcept>
#include <string_view>
#include <variant>
#include <vector>
#include <unistd.h>

using std::map;
using std::runtime_error;
using std::unique_ptr;
using std::make_unique;
using std::cin;
using std::cout;
using std::getline;
using std::deque;
using std::endl;
using std::string;
using std::string_view;
using std::variant;
using std::vector;
//using std::to_string;

typedef vector<string> strings;
typedef variant<double, string> value_t;
typedef vector<value_t> values;
typedef map<string, value_t> varmap_t;
typedef std::function<value_t(values vs)> blang_func;

enum blang_t : char { 
	T_NUL = '0', // represents NULL, or nothing
	T_BAD = '_', // non-matching whitespace. Should not happen, as all input should be made into a token
	T_EOF = 'E', // end of input
	T_REM = '\'', // comment

	T_COM =',', // comma
	T_LRB = '(', // left round bracket
	T_RRB = ')', // right round bracket
	T_REL = '<', // relational operator
	T_NUM = '9', 
	T_ID = 'I', 
	T_ASS = '=', 
	T_MD = '*',  // multiplication or divide
	T_PM = '+',  // plus or minus
	T_STR  = '$' // string
};



typedef struct token { blang_t type; string value; } token;
typedef deque<struct token> tokens;


string to_string(value_t v);

///////////////////////////////////////////////////////////////////////////
// functions

typedef std::function<value_t(values)> blang_func;
typedef map<string, blang_func> blang_funcs_t;

double num(value_t v) { return std::get<double>(v); }

std::tuple<double, double> two_nums(values vs) 
{ 
	if(vs.size()!=2) throw std::runtime_error("#BAD_CALL: Expected 2 arguments");
	double v1 = num(vs[0]), v2 = num(vs[1]);
	return std::make_tuple(v1, v2);
}

//template<typename T>
//value_t num_op(T op, values vs) { auto [v1, v2] = two_nums(vs) ; return op(v1, v2); }

value_t blang_add(values vs) { auto [v1, v2] = two_nums(vs); return v1 + v2; }
value_t blang_eq(values vs) { auto [v1, v2] = two_nums(vs); return v1 == v2; }
value_t blang_ge(values vs) { auto [v1, v2] = two_nums(vs); return v1 >= v2; }
value_t blang_gt(values vs) { auto [v1, v2] = two_nums(vs); return v1 > v2; }
value_t blang_le(values vs) { auto [v1, v2] = two_nums(vs); return v1 <= v2; }
value_t blang_lt(values vs) { auto [v1, v2] = two_nums(vs); return v1 < v2; }
value_t blang_ne(values vs) { auto [v1, v2] = two_nums(vs); return v1 != v2; }
value_t blang_sub(values vs) { auto [v1, v2] = two_nums(vs); return v1 - v2; }
value_t blang_mul(values vs) { auto [v1, v2] = two_nums(vs); return v1 * v2; }
value_t blang_div(values vs) { auto [v1, v2] = two_nums(vs); return v1 / v2; }
//value_t blang_add(values vs) { return num_op(std::plus<double,double>, vs); }

value_t blang_print(values vs)
{
	for(int i= 0; i < vs.size(); i++) {
		cout << to_string(vs[i]);
		if(i+1<vs.size()) cout << " ";
	}
	cout << "\n";
	return 0;
}

value_t blang_lines(values vs)
{
	cin.peek();
	return cin.eof() ? 0 : 1;

}

value_t blang_readln(values vs)
{
	string line;
	getline(cin, line);
	return line;
}

blang_funcs_t blang_funcs = {
	{"<", blang_lt},
	{"<=", blang_le},
	{">", blang_gt},
	{">=", blang_ge},
	{"==", blang_eq},
	{"!=", blang_ne},
	{"+", blang_add},
	{"/", blang_div},
	{"*", blang_mul},
	{"-", blang_sub},

	{"lines", blang_lines},
	{"print", blang_print},
	{"readln", blang_readln}
};


// functions
///////////////////////////////////////////////////////////////////////////

static tokens g_tokens;
static token nextsymb;

map<string, double> varmap; // variables and values

double var_value(string varname) 
{
	auto it = varmap.find(varname);
	double val = 0;
	if(it != varmap.end()) val = varmap[varname];
	return val;
}

tokens tokenise(const string& str)
{
	tokens result;

	//std::string str = " hello how are 2 * 3 you? 123 4567867*98";

	// use std::vector instead, we need to have it in this order
	std::vector<std::pair<string, blang_t>> v
	{
		{"[0-9]+" 			, T_NUM},
		{"[a-z]+"			, T_ID},
		{":="				, T_ASS},
		{"\\("				, T_LRB},
		{"\\)"				, T_RRB},
		{"\\,"				, T_COM},
		{"\\+|\\-"      		, T_PM},
		{"\\*|/"        		, T_MD},
		{"'.*\\n"       		, T_REM},
		{"<=?|>=?|==|!="		, T_REL},
		{"\"(?:[^\"\\\\]|\\\\.)*\""	, T_STR},
		{"\\S+"	        		, T_BAD}
	};

	std::string reg;

	for(auto const& x : v)
		reg += "(" + x.first + ")|"; // parenthesize the submatches

	reg.pop_back();
	//std::cout << reg << std::endl;

	std::regex re(reg);
	auto words_begin = std::sregex_iterator(str.begin(), str.end(), re);
	auto words_end = std::sregex_iterator();

	for(auto it = words_begin; it != words_end; ++it)
	{
		size_t index = 0;

		for( ; index < it->size(); ++index)
			if(!it->str(index + 1).empty()) // determine which submatch was matched
				break;

		struct token toke{v[index].second, it->str()};
		if(toke.type == T_BAD)
			throw runtime_error("Lexical analysis: unhandled token: " + toke.value);
		if(toke.type == T_REM) continue; // just ignore comments

		result.push_back(toke);
	}

	return result;

}

token yylex()
{
	nextsymb = token{T_EOF, ""};
	if( g_tokens.size() == 0) return nextsymb;
	nextsymb = g_tokens[0];
	g_tokens.pop_front();
	return nextsymb;
}


void checkfor(const string& expecting)
{
	if(expecting != nextsymb.value) {
		string msg = "Expecting " + expecting + ", found " + nextsymb.value;
		throw std::runtime_error(msg);
	}
	nextsymb = yylex();
}

///////////////////////////////////////////////////////////////////////////
// ARITHMETIC
// Adopt the algorithm at 
// https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm#classic
// for computing arithmetic
//
// Here's the original derivations
//  E --> T {( "+" | "-" ) T}
//  T --> F {( "*" | "/" ) F}
//  F --> P ["^" F]
//  P --> v | "(" E ")" | "-" T
//
// Here's mine:
// I extend BNF with the notion of a function, prefixed by &
// {} zero or more repetitions
// [] optional
//  &M(o, X) --> X {o X} // e.g. &M(("+"|"-"), T) --> T { ("+"|"-") T }
//  E --> &M(( "<" | "<=" | ">" | ">=" | "==" | "!=" ), R)
//  R --> &M(( "+" | "-" ), T)
//  T --> &M(( "*" | "/" ), F)
//  F --> ["+"|"-"] (v | "(" E ")")

class Def;
class Factor;
class For;
class FuncCall;
class If;
class Let;
class While;

template<typename T>
class Precedence {
	public: vector<T> operands; vector<blang_func> fops;
};

typedef Precedence<Factor> Term;
typedef Precedence<Term> Relop;
typedef Precedence<Relop> Expression;

class Variable { public: string name; };
typedef variant<Expression,Def,If,Let,For,While> Statement;
typedef vector<Statement> Statements;
class FuncCall { public: string name; vector<Expression> exprs; };
class Factor { public: char sign = '+' ; variant<value_t, Expression, FuncCall, Variable> factor; };
class If { public: Expression condition; Statements consequent, alternative; };
class Let { public: string varname; Expression expr; };
class For { public: string varname; Expression from, to; Statements statements; };
class While { public: Expression condition; Statements statements; };
class Def { public: string name; strings args; Statements statements; } ;
class Program { public: Statements statements; };

string curr(tokens& tokes) { return tokes.empty() ? "" : tokes.front().value; }
token take(tokens& tokes) { token toke = tokes.front(); tokes.pop_front(); return toke; }
string take_yytext(tokens& tokes) { token toke{take(tokes)}; return toke.value; }
void advance(tokens& tokes) { tokes.pop_front(); }
bool is_next(const tokens& tokes, string expected) { return tokes.size()<2 ? false : tokes[1].value == expected; }

void require(tokens& tokes, string required) 
{ 
	auto toke = take(tokes);
	string found = toke.value; 
	if(found != required)
		throw std::runtime_error("#PARSE_ERR: Required:" + required + ",found:" + found);
}

Expression make_expression(tokens& tokes);
Statement make_statement(tokens& tokes);
value_t eval(varmap_t& vars, Expression e);
value_t eval(varmap_t& vars, Statement statement);
value_t eval(varmap_t& vars, Statements statements);
value_t eval(varmap_t& vars, value_t v);

template<typename T>
void make_funcargs(tokens& tokes,T make)
{
	require(tokes, "(");
	if(curr(tokes) != ")") {
		make(tokes);
		while(curr(tokes) == ",") {
			advance(tokes);
			make(tokes);
		}
	}
	require(tokes, ")");


}

FuncCall make_funccall(string name, tokens& tokes)
{
	//cout << "make_funccall()\n";
	FuncCall fn;
	fn.name = name;
	auto make = [&fn](tokens& tokes) { fn.exprs.push_back(make_expression(tokes)); };
	make_funcargs(tokes, make);
	return fn;
}


Variable make_variable(string name, tokens& tokes)
{
	//cout << "make_variable()\n";
	Variable var;
	var.name = name;
	return var;
}


Factor make_factor(tokens& tokes)
{
	Factor f;
	token toke = take(tokes);

	// optional sign
	if((toke.value == "+") || (toke.value == "-")) {
		f.sign = toke.value[0];
		toke = take(tokes);
	}

	//cout << "make_factor():token" << toke.value  << "," << toke.type << "\n";
	switch(toke.type) {
		case T_LRB:
			f.factor = make_expression(tokes);
			require(tokes, ")");
			break;
		case T_NUM:
			f.factor = std::stod(toke.value);
			break;
		case T_STR:
			f.factor = toke.value.substr(1,toke.value.size()-2);
			break;
		case T_ID:
			//cout << "make_factor():T_ID:toke.value:" << toke.value << "\n";
			if(curr(tokes) ==  "(")
				f.factor = make_funccall(toke.value, tokes);
			else
				f.factor = make_variable(toke.value, tokes);
			break;
		default:
			throw std::logic_error("make_factor() unhandled type:" 
					+ string{toke.type} + ",value:" + toke.value);
	}
	return f;
}


template <typename T, typename U>
T parse_multiop(tokens& tokes, std::function<U(tokens&)> make, strings ops)
{
	//strings ops{{"+", "-"}};
	T node;
	node.operands.push_back(make(tokes));
	while(1) {
		string op = curr(tokes);
		strings::iterator opit = std::find(ops.begin(), ops.end(), op);
		if(opit == ops.end()) break;
		tokes.pop_front();
		auto fop = blang_funcs.find(op)->second;
		node.fops.push_back(fop);

		node.operands.push_back(make(tokes));
	}
	return node;
}
Term make_term(tokens& tokes) { return parse_multiop<Term,Factor>(tokes, make_factor, {"*", "/"}); }
Relop make_relop(tokens& tokes) { return parse_multiop<Relop,Term>(tokes, make_term, {"+", "-"}); }
Expression make_expression(tokens& tokes) { return parse_multiop<Expression, Relop>(tokes, make_relop, 
		{">", ">=", "<", "<=", "==", "!="}); }


If make_if(tokens& tokes)
{
	If an_if;
	require(tokes, "if");
	an_if.condition = make_expression(tokes);
	require(tokes, "then");

	int num_elses = 0;
	while(curr(tokes) != "fi") {
		if(curr(tokes) == "else") { num_elses++; advance(tokes); }
		switch(num_elses) {
			case 0:
				an_if.consequent.push_back(make_statement(tokes));
				break;
			case 1:
				an_if.alternative.push_back(make_statement(tokes));
				break;
			default:
				throw std::runtime_error("Unexpected else in alternative branch of 'if'");
		}
	}
	require(tokes, "fi");
	return an_if;
}

Let make_let(tokens& tokes)
{
	require(tokes, "let");
	Let let;
	let.varname = take(tokes).value;
	//cout << "make_let():varname:" << let.varname << "\n";
	require(tokes, ":=");
	let.expr = make_expression(tokes);
	return let;
}

Statements collect_statements(tokens& tokes, const string& terminator)
{
	Statements stmts;
	while(curr(tokes) != terminator)
		stmts.push_back(make_statement(tokes));
	return stmts;
}

For make_for(tokens& tokes)
{
	require(tokes, "for");
	For a_for;
	a_for.varname = take(tokes).value;
	cout << "make_for()::varname:" << a_for.varname;
	require(tokes, ":=");
	cout << "make_for()::=from:" << curr(tokes);
	a_for.from = make_expression(tokes);
	require(tokes, "to");
	a_for.to = make_expression(tokes);
	a_for.statements = collect_statements(tokes, "next");
	require(tokes, "next");
	return a_for;
}

While make_while(tokens& tokes)
{
	require(tokes, "while");
	While a_while;
	a_while.condition = make_expression(tokes);
	a_while.statements = collect_statements(tokes, "wend");
	//while(curr(tokes) != "wend")
	//	a_while.statements.push_back(make_statement(tokes));
	require(tokes, "wend");
	return a_while;
}

// turn a user-define function into a blang_func
value_t wrap_def(Def def, values vs)
{
	// bind argument identifiers to values
	if(vs.size() != def.args.size()) {
		auto str = [](int i) { return std::to_string(i);};
		throw std::runtime_error("#FUNC_ARGS:" + def.name 
				+ "():Expected " + str(def.args.size())
				+ " args, got " + str(vs.size()));
	}
	varmap_t vars;
	for(int i=0; i<vs.size(); ++i)
		vars[def.args[i]] = vs[i];

	//value_t v = 0;
	return eval(vars, def.statements);
	//return v;
}

Def make_def(tokens& tokes)
{
	require(tokes, "def");
	Def def;
	def.name = take(tokes).value;
	auto make = [&def](tokens& tokes) { 
		auto toke = take(tokes);
		if(toke.type != T_ID)
			throw std::runtime_error("#PARSE: def of " + def.name 
					+ "has non-var arg " + toke.value);
		def.args.push_back(toke.value); 
	};
	make_funcargs(tokes, make);
	def.statements = collect_statements(tokes, "fed");

	using namespace std::placeholders;
	blang_funcs[def.name] = std::bind(wrap_def, def, _1);
	return def;
}

Statement make_statement(tokens& tokes)
{
	static const map<string, std::function<Statement(tokens&)>> commands = {
		{"def",   make_def},
		{"if",    make_if},
		{"for",   make_for},
		{"let",   make_let},
		{"while", make_while}
	};

	Statement stm;
	auto it = commands.find(curr(tokes));
	if(it != commands.end()) {
		auto make = it->second;
		stm = make(tokes);
	}
	//} else if(curr(tokes) == "def")
	//	make_def(tokes);
	else
		stm = make_expression(tokes);
	return stm;

}


Program make_program(tokens& tokes)
{
	Program prog;
	while(!tokes.empty()) 
		prog.statements.push_back(make_statement(tokes));
	return prog;
}

///////////////////////////////////////////////////////////////////////////
// eval



value_t eval(varmap_t& vars, FuncCall fn)
{
	values vs;
	for(const auto&e: fn.exprs)
		vs.push_back(eval(vars, e));

	auto it = blang_funcs.find(fn.name);
	if(it == blang_funcs.end())
		throw std::runtime_error("#UNK_FUNC:" + fn.name);
	blang_func f = it->second;
	return f(vs);
}

value_t eval(varmap_t& vars, Variable var)
{
	auto it = vars.find(var.name);	
	if(it != vars.end()) {
		//cout << "eval<Variable>():Found:" + var.name +",value:" + to_string(it->second) << "\n";
		return it->second;
	} else
		return 0;

}

value_t eval(varmap_t& vars, value_t v) { return v; }

template<typename T>
bool eval_factor(varmap_t& vars, Factor f, value_t& v)
{
	//bool hit = false;
	//value_t v = 0;
	if(std::holds_alternative<T>(f.factor)) {
		//hit = true;
		v = eval(vars, std::get<T>(f.factor));

		if((f.sign == '-') && std::holds_alternative<double>(v)) 
			v = - std::get<double>(v);
		return true;
	}
	return false;
}


value_t eval(varmap_t& vars, Factor f)
{ 
       	value_t v;
	bool hit = eval_factor<value_t>(vars, f, v) 
		|| eval_factor<Expression>(vars, f, v)
		|| eval_factor<FuncCall>(vars, f, v)
		|| eval_factor<Variable>(vars, f, v);
	if(!hit)
		throw std::runtime_error("eval<Factor>(): unhandled alternative");
	return v;
}

template<class T>
value_t eval_multiop(varmap_t& vars, T expr)
{
	assert(expr.operands.size()>0);
	value_t result = eval(vars, expr.operands[0]);
	for(int i = 0; i< expr.fops.size() ; i++) {
		auto& fop = expr.fops[i];
		result = fop({result, eval(vars, expr.operands[i+1])});
	}
	return result;
}
value_t eval(varmap_t& vars, Term t) { return eval_multiop(vars, t); }
value_t eval(varmap_t& vars, Relop r) { return eval_multiop(vars, r); }
value_t eval(varmap_t& vars, Expression e) { return eval_multiop(vars, e); }


string to_string(value_t v)
{
	if(std::holds_alternative<double>(v)) {
		return std::to_string(std::get<double>(v));
	} else if(std::holds_alternative<string>(v)) {
		return std::get<string>(v);
	} else {
		throw std::logic_error("to_string() unhandled value_t");
	}
}

value_t eval(varmap_t& vars, Let let)
{
	vars[let.varname] = eval(vars, let.expr);
	return 0;
}

value_t eval(varmap_t& vars, Def def)
{
	// The function was actually created by make_def()
	// and put in blang_funcs, so there is nothing to do here
	return 0;
}
template<typename T>
bool eval_holder(varmap_t& vars, Statement statement, value_t& v)
{
	if(std::holds_alternative<T>(statement)) {
		v = eval(vars, std::get<T>(statement));
		//cout << "eval_holder():" << to_string(v) << "\n";
		return true;
	}
	return false;
}

value_t eval(varmap_t& vars, Statements statements)
{
	value_t ret;
	for(auto& s: statements) {
		// use short-circuitng to evaluate the potential types of statements
		bool executed = eval_holder<Expression>(vars, s, ret) 
			|| eval_holder<Def>(vars, s, ret)
			|| eval_holder<If>(vars, s, ret)
			|| eval_holder<Let>(vars, s, ret)
			|| eval_holder<For>(vars, s, ret)
			|| eval_holder<While>(vars, s, ret);
		if(!executed)
			std::logic_error("eval<Program>(): Unhandled statement type");
	}
	return ret;
}

value_t eval(varmap_t& vars, For a_for)
{
	double i = num(eval(vars, a_for.from));
	double to = num(eval(vars, a_for.to));
	while(i <= to) {
		vars[a_for.varname] = i;
		eval(vars, a_for.statements);
		i++;
	}
	return 0;
}

value_t eval(varmap_t& vars, If an_if)
{
	if(num(eval(vars, an_if.condition)))
		eval(vars, an_if.consequent);
	else
		eval(vars, an_if.alternative);
	return 0;
}

value_t eval(varmap_t& vars, While a_while)
{
	//cout << "eval<While>() called\n";
	while(1) {
		value_t test = eval(vars, a_while.condition);
		//cout << "eval<While>():test:" << num(test) << "\n";
		if(num(test) == 0) break;
		eval(vars, a_while.statements);
	}
	return 0;
}


value_t eval(varmap_t& vars, Program prog)
{
	eval(vars, prog.statements);
	return 0;
}


// ARITHMETIC END
///////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////







void scanner()
{
	varmap_t vars;
	Program prog{make_program(g_tokens)};
	eval(vars, prog);
}

void print_tokeniser_report()
{
	cout << "=== TOKENISER REPORT ===\n";
	for(const auto& t: g_tokens) 
		cout <<  t.type << "\t" << t.value <<"\n";
	cout << "=== END TOKENISER REPORT ===\n\n";
}

int main(int argc, char** argv)
{
	// parse command-line options
	string filename;
	int c;
	while((c = getopt(argc, argv, "f:")) != -1)
		switch(c) {
			case 'f':
				filename = optarg;
				break;
		}

	std::ifstream ifs;
	if(filename.size()>0) ifs.open(filename);
	std::istream &is = ifs.is_open() ? ifs : std::cin;
	// read inputs
	string line, str3;
	while(getline(is, line)) {
		str3 += line + "\n";
	}
	if(ifs.is_open()) ifs.close();

	g_tokens = tokenise(str3);
	if constexpr(false) print_tokeniser_report();
	scanner();
	return 0;
}
