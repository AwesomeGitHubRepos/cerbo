#include <any>
#include <cassert>
#include <functional>
#include <iostream>

#include <FlexLexer.h>

using std::cout;
using std::endl;


#include "blang.h"

int make_num(int pos)
{
	//using fn_t = std::function<double()>;
	fn_t fn = [=]() {
		double d =  std::any_cast<double>(parsevec[pos]);
		return d;
	};
	parsevec.push_back(fn);
	return parsevec.size() -1;
}

std::string string_at(int pos)
{
	return std::any_cast<std::string>(parsevec[pos]);
}

int make_funcall(int funid, int argid)
{
	fn_t fn = [=] () {
		// TODO some of this could be defined outside of here, maybe; and speed it up
		std::string fn_name = string_at(funid); // e.g. name returned might be pi, or sqrt
		cout << "make_funcall:fn_name:" << fn_name << endl;
		fn1_t f = funcmap[fn_name];
		values vs; // TODO needs to be created from argid
		return f(vs);
	};
	parsevec.push_back(fn);
	return parsevec.size() -1;
}


int make_concat(int str1, int str2)
{
	//using fn_t = std::function<std::string()>;
	fn_t fn = [=]() { 
		//cout << "Given: " << str1 << " " << str2 << "\n";
		auto s1 =  std::any_cast<std::string>(parsevec[str1]);
		auto s2 =  std::any_cast<std::string>(parsevec[str2]);
		return s1 + s2;
	};
	parsevec.push_back(fn);
	return parsevec.size() -1;
}

void yyerror(const char* s) {
	cout << "Parse error:" <<  s << endl;
	exit(1);
}

yyFlexLexer lexer; // = new yyFlexLexer;

int yylex()
{
	return lexer.yylex();
}


value_t do_pi(values vs)
{
	return 3.14159265359;
}

int main()
{
	extern int yyparse();

	auto ret = yyparse();
	assert(ret == 0);

	// evaluate the top node
	//using fn_t = std::function<value_t()>;
	//cout << "parser stack size:" << parsevec.size() << "\n";
	//fn_t root = std::any_cast<fn_t>(parsevec[parsevec.size()-1]);
	fn_t root = std::any_cast<fn_t>(parsevec.back());
	value_t val = root();

	//std::string result;
	cout << "Computer says:";
	if(std::holds_alternative<double>(val))
		cout << std::get<double>(val);
	else if(std::holds_alternative<std::string>(val))
		cout << std::get<std::string>(val);
	else
		assert(false);

	cout << endl;
	//if(roo

	return 0;
}
