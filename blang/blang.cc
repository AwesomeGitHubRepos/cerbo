#include <cassert>
//#include <deque>
#include <exception>
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

enum blang_t { 
	T_NUL, // represents NULL, or nothing
	T_BAD, // non-matching whitespace. Should not happen, as all input should be made into a token
	T_EOF, // end of input
	T_REM, // comment

	T_COM, // comma
	T_LRB, // left round bracket
	T_RRB, // right round bracket
	T_REL, // relational operator
	T_NUM, T_ID, T_ASS, 
	T_MD,  // multiplication or divide
	T_PM,  // plus or minus
	T_STR  // string
};



typedef struct token { blang_t type; string value; } token;
typedef deque<struct token> tokens;

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
		{"<=|<|>=|>|==|!="		, T_REL},
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
//  E --> R {( "<" | "<=" | ">" | ">=" | "==" | "!=" ) R}
//  R --> T {( "+" | "-" ) T}
//  T --> P {( "*" | "/" ) P}
//  P --> v | "(" E ")" | "-" T

class Factor;
//class Relop;
//class Term;




template<typename T>
class Precedence {
	public: vector<T> operands; vector<blang_func> fops;
};

//class Term : public Precedence<Factor> {};
typedef Precedence<Factor> Term;
typedef Precedence<Term> Relop;
typedef Precedence<Relop> Expression;

class Factor { public: variant<value_t, Expression> factor; };

token take(tokens& tokes) { token toke = tokes.front(); tokes.pop_front(); return toke; }

Factor make_factor(tokens& tokes)
{
	Factor f;
	token toke = take(tokes);
	switch(toke.type) {
		case T_NUM:
			f.factor = std::stod(toke.value);
			break;
		case T_STR:
			f.factor = toke.value;
			break;
		default:
			throw std::logic_error("make_factor() unhandled type:" + std::to_string(toke.type));
	}
	return f;
}


template <typename T, typename U>
T parse_multiop(tokens& tokes, std::function<U(tokens&)> make, const strings& ops)
{
	T node;
	node.operands.push_back(make(tokes));
	return node;
}
Term make_term(tokens& tokes) { return parse_multiop<Term,Factor>(tokes, make_factor, {"*", "/"}); }
Relop make_relop(tokens& tokes) { return parse_multiop<Relop,Term>(tokes, make_term, {"+", "-"}); }
Expression make_expression(tokens& tokes) { return parse_multiop<Expression, Relop>(tokes, make_relop, {">", "<"}); }


///////////////////////////////////////////////////////////////////////////
// eval


value_t eval(varmap_t& vars, Factor f) { return std::get<value_t>(f.factor); } // TODO may need to eval other althernatives

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




// ARITHMETIC END
///////////////////////////////////////////////////////////////////////////





///////////////////////////////////////////////////////////////////////////







void scanner()
{
	Expression e{make_expression(g_tokens)};
	varmap_t vars;
	cout << to_string(eval(vars, e)) << "\n";

}

void print_tokeniser_report()
{
	cout << "=== TOKENISER REPORT ===\n";
	for(const auto& t: g_tokens) 
		cout <<  t.type << "\t" << t.value <<"\n";
	cout << "=== END TOKENISER REPORT ===\n\n";
}

int main()
{
	// read inputs
	string line, str3;
	while(getline(cin, line)) {
		str3 += line + "\n";
	}

	g_tokens = tokenise(str3);
	if constexpr(false) print_tokeniser_report();
	scanner();
	return 0;
}
