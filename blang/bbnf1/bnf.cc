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

#include <any>
#include <cassert>
#include <cstdio>
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
#include <sstream>
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

///////////////////////////////////////////////////////////////////////////
// definition of the grammar

std::string bbnf_grammar = R"(
	ruleset = *rule;
	rule = @rulename @eq expr @semi;
	expr = seq *(@bar seq) ;
	seq  = +(?@qty basic);
	basic = @terminal | @rulename | bracket;
	bracket = @lrb expr @rrb;
)";

std::regex Re(const std::string& patt) 
{ 
	std::string patt1 = "(" + patt + ")(.*)";
	return std::regex{patt1}; 
}

typedef std::vector<std::pair<std::regex, std::string_view>> deflexes_t;

///////////////////////////////////////////////////////////////////////////

typedef vector<string> strings;

typedef struct bblexeme { std::string_view bbltype; std::string bbtext; } bblexeme_t;
typedef std::deque<bblexeme_t> bblexemes_t;

bblexemes_t tokenise(const string& str)
{
	bblexemes_t result;
	auto push = [&](std::string_view t, std::string s) { 
		bblexeme_t lex{t, s};
		result.push_back(lex);
	};

	std::stringstream ssin{str};
	//ssin.peek();
	char c;
	while(ssin.get(c)) {
		switch(c) {
			case ' ':
			case '\t':
			case '\r':
			case '\n':
				break;
			case '(':
				push("lrb", "(");
				break;
			case ')':
				push("rrb", ")");
				break;
			case '?':
				push("qty", "?");
				break;
			case '*':
				push("qty", "*");
				break;
			case '+':
				push("qty", "+");
				break;
			case '=':
				push("eq", "=");
				break;
			case '|':
				push("bar", "|");
				break;
			case ';':
				push("semi", ";");
				break;
				
			case '@':
				{
					std::stringstream ss;
					while(isalpha(ssin.peek()) !=0) { ssin.get(c) ; ss << c; }
					push("terminal", ss.str());
				}
				break;
				
			default:
			       	assert(isalpha(c) != 0);
				{
					std::stringstream ss;
					ss << c;
					while(isalpha(ssin.peek())  !=0) { ssin.get(c); ss << c; }
					push("rulename", ss.str());
				}
				break;
			
		}

	}

	if(true) { //dump lexemes
		for(auto const& lex: result)
			cout << lex.bbltype << ":" << lex.bbtext << "\n";
	}
	return result;
}

///////////////////////////////////////////////////////////////////////////
// scanner

typedef std::optional<bblexeme_t> opt_bblexeme_t;

opt_bblexeme_t peek(const bblexemes_t& bblexemes)
{
	if(bblexemes.size() >0)
		return opt_bblexeme_t{bblexemes.front()};
	else
		return {};
}
void make_bracket(bblexemes_t& bblexemes)
{
	match = false;
	auto lex1 = peek(bblexemes);
	if(lex1.bbltype == "terminal")
		return rulemap["terminal"](lex1);
	else if(lex1.bbltype == "rulename")
		return rulemap("rulename")(lex1);

}
void bbparse(bblexemes_t& bblexemes)
{
	//zero_or_more(make_rule, bblexemes);
	make_bracket(bblexemes);
}

///////////////////////////////////////////////////////////////////////////
int main()
{
	tokenise(bbnf_grammar); 
	return 0;
}
