#include <deque>
#include <exception>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <regex>
#include <iterator>
#include <stdexcept>
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
using std::vector;
using std::to_string;

// taken from https://github.com/eantcal/nubasic/blob/master/include/nu_eval_expr.h
//variant_t eval_expr(rt_prog_ctx_t& ctx, std::string data);

enum blang_t { 
	T_NUL, // represents NULL, or nothing
	T_BAD, // non-matching whitespace. Should not happen, as all input should be made into a token
	T_EOF, // end of input

	T_COM, // comma
	T_LRB, // left round bracket
	T_RRB, // right round bracket
	T_NUM, T_ID, T_ASS, 
	T_MD,  // multiplication or divide
	T_PM   // plus or minus
};

class BlangCode {
	public:
		//BlangCode() {;};
		virtual ~BlangCode() {};
		virtual void eval() = 0;
};

typedef struct token { blang_t type; string value; } token;
typedef deque<struct token> tokens;

static tokens g_tokens;
static token nextsymb;

//class BlangExpr;
//map<string, unique_ptr<BlangExpr>> varmap; // variables and values
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
		{"[0-9]+" , T_NUM} ,
			{"[a-z]+"	, T_ID},
			{":="		, T_ASS},
			{"\\("		, T_LRB},
			{"\\)"		, T_RRB},
			{"\\,"		, T_COM},
			{"\\+|\\-"      , T_PM},
			{"\\*|/"        , T_MD},
			{"\\S+"	        , T_BAD}
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

		//std::cout << it->str() << "\t" << v[index].second << std::endl;
		//struct token toke = {.type = v[index].second, .value = it->str()};
		struct token toke{v[index].second, it->str()};
		if(toke.type == T_BAD)
			throw runtime_error("Lexical analysis: unhandled token: " + toke.value);

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

/*
void syntax_error(const string& expecting, const token& toke, const string& where)
{
	string msg = "Syntax error. Expecting " + expecting + ", got " + toke.value + " in " + where;
	throw std::runtime_error(msg);
}
*/

void checkfor(const string& expecting)
{
	if(expecting != nextsymb.value) {
		string msg = "Expecting " + expecting + ", found " + nextsymb.value;
		throw std::runtime_error(msg);
	}
	nextsymb = yylex();
	//checkfor(expecting, get(tokes), where);
}

/*
class BlangOp: public BlangCode {
	public:
		BlangOp() {
			op = nextsymb.value;
			if(!(nextsymb.value == "+" || nextsymb.value == "-"))
				throw runtime_error("BlangOp error. Expected '+' or '-', got "
						+ nextsymb.value);
			nextsymb = yylex();
			cout << "BlangOp nextsymb is " << nextsymb.value << endl;
		}
		void eval() {}
		~BlangOp() {}
		double get_sign() { return op == "+" ? 1 : -1;}
	private:
		string op;
};
*/

///////////////////////////////////////////////////////////////////////////
// ARITHMETIC
// Adopt the algorithm at 
// https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm#classic
// for computing arithmetic
//
//  E --> T {( "+" | "-" ) T}
//  T --> F {( "*" | "/" ) F}
//  F --> P ["^" F]
//  P --> v | "(" E ")" | "-" T

class BlangP;

class BlangT;

class BlangE {
	public:
		BlangE();
		double get_value();
	private:
		vector<unique_ptr<BlangT>> Ts;
		vector<string> ops{"+"};

};

class BlangT {
	public:
		BlangT();
		double get_value();
	private:
		vector<unique_ptr<BlangP>> Ps;
		vector<string> ops{"*"};

};
class BlangP {
	public:
		BlangP() {
		       	//  P --> v | "(" E ")" | "-" T
			if(nextsymb.type == T_NUM || nextsymb.type == T_ID ) {
				type = 'V'; // variable name
				if(nextsymb.type == T_NUM) type = 'N'; // numeric
				toke = nextsymb;
				nextsymb = yylex();
			} else if(nextsymb.value == "(") {
				type = 'E';
				nextsymb = yylex();
				expr.push_back(make_unique<BlangE>());
				checkfor(")");
			} else if(nextsymb.value == "-") {
				type = 'T';
				nextsymb = yylex();
				term.push_back(make_unique<BlangT>());
			} else {
				string msg = "BlangT syntax error with token " + nextsymb.value;
				throw runtime_error(msg);
			}
		}

		double get_value() {
			switch(type) {
				case 'V': return var_value(toke.value);
				case 'N': return stod(toke.value);
				case 'E': {
						  double d = 0;
						  for(int i = 0; i< expr.size(); i++)
							  d += expr[i]->get_value();
						  return d;
					  }
				case 'T': {
						  double d = 0;
						  for(int i = 0; i < term.size(); i++)
							  d += term[i]->get_value();
						  return -d;
					  }
				default:
					  throw runtime_error("BlangP get_value of unrecognised type: " + type);
			}
		}

	private:
		char type = '?';
		token toke;
		vector<unique_ptr<BlangE>> expr;
		vector<unique_ptr<BlangT>> term;
};

BlangT::BlangT() {
	//  T --> F {( "*" | "/" ) F}   (but using P instead of F)
	Ps.push_back(make_unique<BlangP>());
	while(nextsymb.value == "*" || nextsymb.value == "/") {
		ops.push_back(nextsymb.value);
		nextsymb = yylex();
		Ps.push_back(make_unique<BlangP>());
	}
}
double BlangT::get_value() {
	double d = 1;
	for(int i = 0; i < Ps.size(); i++) {
		if(ops[i] == "*")
			d = d * Ps[i]->get_value();
		else
			d = d / Ps[i]->get_value();
	}
	return d;
}


BlangE::BlangE() {
	//  E --> T {( "+" | "-" ) T}
	Ts.push_back(make_unique<BlangT>());
	while(nextsymb.value == "+" || nextsymb.value == "-") {
		ops.push_back(nextsymb.value);
		nextsymb = yylex();
		Ts.push_back(make_unique<BlangT>());
	}			
}

double BlangE::get_value(){
	double d = 0;
	for(int i = 0 ; i<Ts.size(); i++) {
		double sgn = ops[i] == "+" ? 1 : -1;
		double v = Ts[i]->get_value();
		//cout << "BlangE::get_value i : " << v << "\n";
		d += sgn * v;
	}
	return d;
}

// ARITHMETIC END
///////////////////////////////////////////////////////////////////////////

class BlangExprList : public BlangCode {
	public:
		BlangExprList() {
			checkfor("(");
			//while( nextsymb.val == "," 
			while(true) {
				if(nextsymb.value == ")") break;
				ptrs.push_back(make_unique<BlangE>());
				//add_expr();
				if(nextsymb.value != ",") break;
				nextsymb = yylex();
			}
			checkfor(")");
		}
		void eval() {};
		size_t size() const { return ptrs.size(); }
		~BlangExprList() {}
		string get_value(int i) const { 
			double d = ptrs[i]->get_value();
			return to_string(d);
		}

		//vector<unique_ptr<BlangExpr>> get_values() const { return ptrs;}
		
	private:
		vector<unique_ptr<BlangE>> ptrs;

};
/*
BlangExpr expression()
{
	//token toke = get(tokes);
	cout << "TODO expressione\n";
	return BlangExpr();
}
*/

class BlangPrint: public BlangCode {
	public:
		//BlangPrint(const BlangExpr& e): _e{e} {};
		//BlangPrint() {
		//	_e = 
		//       	_e{e} {};
		BlangPrint() {
			//cout << "BlangPrint says hello\n";
			ptr = make_unique<BlangExprList>();
			//checkfor("(");
			//_e = make_unique<BlangExpr>();
			//checkfor(")");
		}

		void eval() { 
			cout << "BlangPrint: ";
			/*
			for(const auto& e: _e->get_values())
				cout << e->get_value() << " ~ ";
				*/
			for(int i = 0; i< ptr->size(); ++i) {
				cout << ptr->get_value(i);
				if( i +1 < ptr->size()) cout << " ~ ";
			}

		       	cout <<  "\n" ; 
		}

		~BlangPrint() {
		       //cout << "Bye from BlangPrint\n"; 
		}
	private:
		unique_ptr<BlangExprList> ptr = nullptr;
		//unique_ptr<BlangCode> ptr = nullptr;
};

class BlangLet: public BlangCode { // assignment statement
	public:
		BlangLet() {
			varname = nextsymb.value;
			nextsymb = yylex();
			checkfor(":=");
			//cout << "BlangLet checkpoint nextysmb: " << nextsymb.value << endl;
			ptr = make_unique<BlangE>();
			//cout << "BlangLet checkpoint nextysmb: " << nextsymb.value << endl;
		}

		void eval() {
			varmap[varname] = ptr->get_value();
			//cout << "TODO BlangLet eval on variable " << varname << "\n";
		}

		private:
		string varname;
		unique_ptr<BlangE> ptr;

};

///////////////////////////////////////////////////////////////////////////
class BlangStmt: public BlangCode { // some statement
	public:
		BlangStmt();
		void eval();
	private:
		unique_ptr<BlangCode> stmt = nullptr;
};

class BlangProg : BlangCode {
	public:
		BlangProg();
		void eval();
	private:
		vector<unique_ptr<BlangStmt>> stmts;
};
class BlangFor: public BlangCode { // for loop
	public:
		BlangFor() {
			//variable name required
			varname = nextsymb.value;
			if(nextsymb.type != T_ID)
				throw runtime_error("BlangFor: expecting ID but found " + nextsymb.value);
			nextsymb = yylex();

			checkfor(":=");
			from_ptr = make_unique<BlangE>();
			checkfor("to");
			to_ptr = make_unique<BlangE>();
			while(nextsymb.value != "next") {
				stmts.push_back(make_unique<BlangStmt>());
			}
			nextsymb = yylex();
		}

		void eval() {
			varmap[varname] = from_ptr->get_value();
			for(int i = from_ptr->get_value(); i <= to_ptr->get_value(); i++) {
				varmap[varname] = i;
				for(auto& s: stmts)
					s->eval();
			}
		}
	private:
		string varname;
		unique_ptr<BlangE> from_ptr, to_ptr;
		vector<unique_ptr<BlangStmt>> stmts;
};


BlangStmt::BlangStmt() {
	string sym = nextsymb.value;
	nextsymb = yylex();
	if(sym == "print")
		stmt = make_unique<BlangPrint>();
	else if(sym == "let")
		stmt = make_unique<BlangLet>();
	else if(sym == "for")
		stmt = make_unique<BlangFor>();
	else
		throw runtime_error("Statement unknown token: " + sym);
}

void BlangStmt::eval()
{ 
	stmt->eval(); 
}

BlangProg::BlangProg()
{ 
	while(nextsymb.type != T_EOF)
	{
		stmts.push_back(make_unique<BlangStmt>());
	}
}

void BlangProg::eval()
{
       	for(auto& s: stmts) s->eval();
}


void scanner()
{
	vector<unique_ptr<BlangCode>> v;
	nextsymb = yylex();
	BlangProg prog; // scan
	cout << "Finished scanning\n";
	prog.eval();
	cout << "Run finished\n";
}

int main()
{
	// read inputs
	string line, str3;
	while(getline(cin, line)) {
		str3 += line + "\n";
	}

	g_tokens = tokenise(str3);

	cout << "=== TOKENISER REPORT ===\n";
	for(const auto& t: g_tokens) 
		cout <<  t.type << "\t" << t.value <<"\n";
	cout << "=== END TOKENISER REPORT ===\n\n";

	scanner();
	return 0;
}
