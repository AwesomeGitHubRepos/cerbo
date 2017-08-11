#include <deque>
#include <exception>
#include <iostream>
#include <string>
#include <regex>
#include <iterator>
#include <stdexcept>
#include <vector>

using std::cout;
using std::deque;
using std::endl;
using std::string;
using std::vector;

enum blang_t { 
	T_NUL, // represents NULL, or nothing
	T_EOF, // end of input
	T_NUM, T_ID, T_ASS, T_OP 
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
			{"[a-z]+" , T_ID},
			{"="      , T_ASS},
			{"\\*|\\+", T_OP}
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
void expression()
{
	//token toke = get(tokes);
	cout << "TODO expressione\n";
}

void scanner()
{
	nextsymb = yylex();
	checkfor("print");
	expression();
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

string str2 = R"( print 42 )";

	g_tokens = tokenise(str2);

	cout << "=== TOKENISER REPORT ===\n";
	for(const auto& t: g_tokens) 
		cout <<  t.type << "\t" << t.value <<"\n";
	cout << "=== END TOKENISER REPORT ===\n\n";

	scanner();
	return 0;
}
