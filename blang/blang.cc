#include <deque>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <regex>
#include <iterator>
#include <stdexcept>
#include <vector>

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

// taken from https://github.com/eantcal/nubasic/blob/master/include/nu_eval_expr.h
//variant_t eval_expr(rt_prog_ctx_t& ctx, std::string data);

enum blang_t { 
	T_NUL, // represents NULL, or nothing
	T_BAD, // non-matching whitespace. Should not happen, as all input should be made into a token
	T_EOF, // end of input

	T_COM, // comma
	T_LRB, // left round bracket
	T_RRB, // right round bracket
	T_NUM, T_ID, T_ASS, T_OP 
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
			{"\\*|\\+"      , T_OP},
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

/*
void checkfor(const string& expecting, const token& toke, const string& where)
{
	if(expecting != toke.value)
		syntax_error(expecting, toke, where);
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

void variable()
{
	//token toke = get(tokes);
	cout << "TODO variable\n";
}

class BlangExpr: public BlangCode {
	public:
		BlangExpr()  {
			_e = nextsymb.value;
			nextsymb = yylex();
		};
		void eval() {}
		~BlangExpr() { cout << "Bye from BlangExpr\n" ; };
		string get_value() const { return _e;}

	private:
		string _e;
};

class BlangExprList : public BlangCode {
	public:
		BlangExprList() {
			checkfor("(");
			//while( nextsymb.val == "," 
			while(true) {
				if(nextsymb.value == ")") break;
				ptrs.push_back(make_unique<BlangExpr>());
				//add_expr();
				if(nextsymb.value != ",") break;
				yylex();
			}
			checkfor(")");
		}
		void eval() {};
		size_t size() const { return ptrs.size(); }
		~BlangExprList() {}
		string get_value(int i) const { return ptrs[i]->get_value(); }

		//vector<unique_ptr<BlangExpr>> get_values() const { return ptrs;}
		
	private:
		vector<unique_ptr<BlangExpr>> ptrs;

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
			cout << "BlangPrint says hello\n";
			_e = make_unique<BlangExprList>();
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
			for(int i = 0; i< _e->size(); ++i) {
				cout << _e->get_value(i);
				if( i +1 < _e->size()) cout << " ~ ";
			}

		       	cout <<  "\n" ; 
		}

		~BlangPrint() { cout << "Bye from BlangPrint\n"; }
	private:
		unique_ptr<BlangExprList> _e = nullptr;
};

void scanner()
{
	//vector<std::unique_ptr<BlangCode> > v;

	//vector<BlangCode*> v;

	vector<unique_ptr<BlangCode>> v;
	nextsymb = yylex();
	while(nextsymb.type != T_EOF)
	{

		checkfor("print");
		//BlangExpr expr = expression();
		//BlangPrint print = BlangPrint(BlangExpr());
		//v.push_back(&print);
		//v.push_back(new BlangPrint());
		v.push_back(make_unique<BlangPrint>());
	}

	// Now run the code
	for(auto& c: v)
		c->eval();

	cout << "Finished parsing\n";
	/*
	   token toke;
	   checkfor("for");
	   variable();
	   checkfor("=");
	   expression();
	   checkfor("to");
	   expression();
	   */
}

int main()
{
	std::string str1 = R"(
for i = 1 to 6
	print i * 2
next
)";

string str2 = R"( print (16, 42 )  print( 43, 44 , 45)     )";

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
