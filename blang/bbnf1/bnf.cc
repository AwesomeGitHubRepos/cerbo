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

typedef struct bblexeme { std::string_view bbltype; std::any bblval; } bblexeme_t;
typedef std::deque<bblexeme_t> bblexemes_t;

class Rule { public: std::string name, Expansion expansion; };

typedef vector<Rule> Rules;

class Bnf { public: Rules rules; };

Rule make_rule(bblexemes_t& bblexemes)
{
	Rule rule;
	rule.name = make_name(bblexemes);
	require_and_skip(bblexemes, "::="sv);
	rule.expansion = make_expansion(bblexemes);
	return rule;
}

Bnf make_bnf(bblexemes_t& bblexemes)
{
	Bnf bnf;
	while(!bblexemes.empty())
		bnf.rules.push_back(make_rule(bblexemes));
	return bnf;
}
