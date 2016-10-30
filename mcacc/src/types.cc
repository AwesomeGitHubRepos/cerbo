/** File containing type information */

#include <tuple>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <algorithm>

#include "common.hpp"
#include "types.hpp"
#include "reusable.hpp"

using namespace std;


string pad_ticker(string ticker) { return pad_right(ticker, 7); }
string pad_ticker(char c) { return nchars(c, 7);}

string pad_gbp(char c) { return nchars(c, 12);}
string pad_gbp(string s) { return pad_left(s, 12); }

string ret_str(double r)
{
	return format_num(r, 7, 2);
}


string ret_str(double num, double denom) {  return ret_str(num/denom*100-100); }
string ret_str(char c) { return nchars(c, 7); }
string ret_str(string s) { return pad_left(s, 7); }

//std::string ret_str(const centis& num, const centis& denom)
//{
//	return ret_str(num.get(), denom.get());
//}


std::string retchg_str(double num, double denom)
{
	return ret_str(num/denom *100);
}

//std::string retchg_str(const centis& num, const centis& denom)
//{
//	return retchg_str(num.get(), denom.get());
//}


//std::string ret_str(const price& num, const price& denom)
//{
//	return ret_str(num.get(), denom.get());
//}
void print_strings(ostream &ost, const strings &strs)
{
	auto line = intercalate(" ", strs);
	ost << line << endl;
}



PeriodTiming period::when (const std::string &dstamp) const
{
	if(dstamp < start_date) return perBefore;
	if(dstamp > end_date) return perAfter;
	return perDuring;
}

bool period::during(const std::string &dstamp) const
{
	return when(dstamp) == perDuring;
}


