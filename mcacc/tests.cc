//#include <chrono>
#include <cmath>
#include <decimal/decimal>
#include <iostream>
//#include <regex>
#include <string>
#include <type_traits>

#include "dec.hpp"
#include "inputs.hpp"
#include <supo_general.hpp>


//using std::regex;
//using std::regex_token_iterator;

using std::cout;
using std::endl;
using std::string;
using namespace supo;

/*
class Timer 
{
	public:
		Timer() { start(); };
		void start() {m_start = std::chrono::high_resolution_clock::now(); };
		void stop() {m_stop = std::chrono::high_resolution_clock::now(); };
		double nanos() { std::chrono::duration<double> d = m_stop - m_start; 
			return std::chrono::duration_cast<std::chrono::nanoseconds>(d).count(); };
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock>
			m_start, m_stop;

};
*/

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

bool operator==(strings lhs, strings rhs)
{
	if(lhs.size() != rhs.size()) return false;
	for(size_t i=0; i< lhs.size(); ++i)
		if(lhs[i] != rhs[i]) return false;
	return true;
}
void check_tokeniser(std::string str, std::vector<std::string> strs)
{
	bool ok = parse::tokenise_line(str) == strs;
	std::string msg = "tokenise_line() <" + str + ">";
	check(ok, msg);
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

/*
strings multi_split(const string& line, char c)
{
	strings result;
	std::size_t prev = 0, pos;
	while((pos = line.find_first_of(c, prev)) != std::string::npos)
	{
		if(pos>prev)
			result.push_back(line.substr(prev, pos-prev));
		prev = pos+1;
	}
	if (prev<line.length())
		result.push_back(line.substr(prev, std::string::npos));
	return result;

}
*/

// TODO definitely reusable!
string char_to_string(char c)
{
	char cstr[] = {c};
	return cstr;
}

/*
strings tokenise_2(const string& line)
{
	constexpr char FS = 0x1C;
	bool found_quote = false;

	char* line_cs = (char *)line.c_str(); // potentially tricky, I guess
	for(int i=0; i< line.size(); ++i){
		char c = line[i];
		if(c == '"') {
			found_quote = true ;
		       	line_cs[i] = FS;
	       	}
		if(found_quote) continue;
		if(c == ' ' || c == '\t') line_cs[i] = FS;
	}
	
	return multi_split(line, FS);
}
void check_tokeniser_2(const string& msg, const string& input, const strings& expected_result)
{
	strings strs = tokenise_2(input);
	if(false) { // for debugging purposes
		for(auto s:strs) cout << "[" << s << "] ";
		cout << endl;
	}

	bool ok = strs == expected_result;
	check(ok, msg);
}
void check_tokeniser_2_all()
{
	check_tokeniser_2("toke2-01", "  how   now brown cow",
			{"how", "now", "brown", "cow"});
	check_tokeniser_2("toke2-02", "how now    \"brown  cow\"",
			{"how", "now", "brown  cow"});

	string cow = "how now\t \"brown cow\"";
	Timer time;
	time.start();
	for(int i =0; i<1000; ++i) parse::tokenise_line(cow);
	time.stop();
	cout << "Elapsed: " << time.nanos() << endl;
	time.start();
	for(int i =0; i<1000; ++i) tokenise_2(cow);
	time.stop();
	cout << "Elapsed: " << time.nanos() << endl;

}
*/

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

	check_decimals();
	//check_tokeniser_2_all();
}
