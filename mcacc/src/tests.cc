#include <cmath>
#include <decimal/decimal>
#include <iostream>
#include <string>
#include <type_traits>

//#include "cpq.hpp"
#include "dec.hpp"
#include "inputs.hpp"
#include "reusable.hpp"



using std::cout;
using std::endl;
using std::string;

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
	//msg +=	c + " in " + src;
	check(s == targ, msg);
}

void check_tokeniser(std::string str, std::vector<std::string> strs)
{
	std::vector<std::string> result = parse::tokenise_line(str);
	std::string msg = "tokenise_line() <" + str + ">";

	bool ok = strs.size() == result.size();
	for(int i = 0; i< strs.size() && ok; i++) {
		if(strs[i] != result[i]) { 
			//std::cout << strs[i] <<result[i] << strs[i].size() << result[i].size() ; 
			//std::cout << strs[i].size() << result[i].size();
			ok = false; 
		}
	}
	check(ok, msg);
}

void check_fundamentals()
{	
	/*
	price p;
	p.set("123.45");
	check_near(p.get(), 123.45, "price 123.45");


	centis c;
	c.set("456.78");
	check_near(c.get(), 45678, "centis 456.78");

	quantity q;
	q.set("37.46");
	check_near(q.get(), 37.46, "quantity 37.46");

	p.reprice(c, q);
	check_near(p.get(), 1219.3806, "reprice");
	
	//recentis(c, p, q);
	c = p*q;
	check_near(c.get(), 45678, "recentis");
	*/
}

void check_decimals()
{
	cout.precision(15);

	currency d1 = currency(12, 34);
	static_assert(true, "always works");
	//assert(d.dp ==2);
	check(d1==d1, "decimal trivial equality");
	currency d2 = currency(12, 35);
	check(d1!=d2, "decimal trivial inequality");
	check(currency(10, 12) + currency(13, 14) == currency(23, 26), "decimal simple addition");
	check_near(currency(10, 12).dbl(), 10.12, "currency 10.12");

	currency c1;
	c1.from_str("34.56");
	check_near(c1.dbl(), 34.56, "currency from str");
	check(c1.stra() == "34.56", "and back to string again");

	quantity q1(10,0);
	price p1 = c1/q1;
	check_near(p1.dbl(), 345.6, "p 345.6 = c 34.56 / q 10.0");
	currency c2 = p1 * q1;
	check(c2.stra() == "34.56", "and back to string again");

	price p2("145.6");
	check(p2.stra() == "145.60000", "price from string");
	//cout << p2.str() << endl;
	
	price p3 = p2;
	check(p3.stra() == p2.stra(), "simple price assignment");

	currency d3 = price("200.4749") * quantity("8889.15787");
	check_near(d3.dbl(), 17820.53, "200.4749 * 8889.15787");



}

void run_all_tests()
{
	check_erase_all("", 'a', "");
	check_erase_all("aaa", 'a', "");
	check_erase_all("alibaba", 'a', "libb");
	check_erase_all("baad", 'a', "bd");

	check_tokeniser("hello world",  { "hello", "world" });
	check_tokeniser("foo  \"baz shazam\"  bar", {"foo", "baz shazam", "bar"});
       	check_tokeniser("    hello  \"joe blogg\"  world   # just a comment   ", {"hello", "joe blogg", "world"});
	check_tokeniser("this is \"tricky disco", {"this", "is", "tricky disco"});

	check_fundamentals();
	check_decimals();
}
