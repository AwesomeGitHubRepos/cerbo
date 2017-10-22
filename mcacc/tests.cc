#include <cmath>
//#include <decimal/decimal>
#include <iostream>
#include <string>
#include <type_traits>

#include "dec.h"
#include "inputs.h"
#include <supo_general.hpp>

using std::cout;
using std::endl;
using std::string;
using namespace supo;

void check(bool ok, std::string msg)
{
	std::string s = ok? "PASS" : "FAIL";
	std::cout << s << " " << msg << std::endl;
}


void check_near(double src, double targ, string msg)
{
	check(fabs(src-targ) <0.0001, msg);
}
void check_erase_all(std::string src, char c, std::string targ)
{
	std::string s = src;
	erase_all(s, c);
	std::string cstr;
       	cstr	= c;
	std::string msg = "erase_all() " + cstr + " in " + src;
	check(s == targ, msg);
}

bool operator==(strings lhs, strings rhs)
{
	if(lhs.size() != rhs.size()) return false;
	for(size_t i=0; i< lhs.size(); ++i)
		if(lhs[i] != rhs[i]) return false;
	return true;
}


void check_decimals()
{
	cout.precision(15);

	currency d1 = currency(12, 34);
	static_assert(true, "always works");
	check(d1==d1, "decimal trivial equality");
	currency d2 = currency(12, 35);
	check(d1!=d2, "decimal trivial inequality");
	check(currency(10, 12) + currency(13, 14) == currency(23, 26), "decimal simple addition");
	check_near(currency(10, 12)(), 10.12, "currency 10.12");

	check_near(price(206.65).value, 206.65, "price 1: 206.65");
	check_near(price(206.65)(), 206.65, "price 1: 206.65");

	currency c1;
	c1.from_str("34.56");
	check_near(c1(), 34.56, "currency from str");
	check(c1.stra() == "34.56", "and back to string again");

	quantity q1(10,0);
	check_near(q1(), 10.0, "qty 10.0");

	price p1 = div(c1, q1);
	check_near(p1(), 345.6, "p 345.6 = c 34.56 / q 10.0");
	currency c2 = mul(p1, q1);
	check(c2.stra() == "34.56", "and back to string again");

	price p2("145.6");
	check(p2.stra() == "145.60000", "price from string");
	//cout << p2.str() << endl;
	
	price p3 = p2;
	check(p3.stra() == p2.stra(), "simple price assignment");

	currency d3 = mul(price("200.4749"),  quantity("8889.15787"));
	check_near(d3(), 17820.53, "200.4749 * 8889.15787");



}


// TODO definitely reusable!
string char_to_string(char c)
{
	char cstr[] = {c};
	return cstr;
}


void run_all_tests()
{
	check_erase_all("", 'a', "");
	check_erase_all("aaa", 'a', "");
	check_erase_all("alibaba", 'a', "libb");
	check_erase_all("baad", 'a', "bd");
	check_decimals();
}
