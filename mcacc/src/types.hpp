#pragma once
#include <cstdio>
#include <string>
#include <vector>
#include <map>

//class centis; // forward declaration
//class price; // ditto

typedef std::vector<std::string> strings;
typedef std::string dstamp_t;

std::string pad_ticker(std::string ticker);
std::string pad_ticker(char c);

std::string pad_gbp(char c); 
std::string pad_gbp(std::string s);
std::string ret_str(double r);
std::string ret_str(double num, double denom); 
//std::string ret_str(const centis& num, const centis& denom); 
std::string ret_str(const price& num, const price& denom); 
std::string ret_str(char c);
std::string ret_str(std::string s);
std::string retchg_str(double num, double denom);
//std::string retchg_str(const centis& num, const centis& denom);

void print_strings(std::ostream &ost, const strings &strs);





enum PeriodTiming { perBefore, perDuring, perAfter };
class period {
	public:
		period() {};
		std::string start_date = "0000-01-01";
		std::string end_date = "3000-12-31";
		PeriodTiming when(const std::string &dstamp) const;
		bool during(const std::string &dstamp) const;		
};

typedef std::vector<std::string>  vs_t;
typedef std::vector<std::vector<std::string> > vecvec_t;
