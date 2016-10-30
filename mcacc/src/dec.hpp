#pragma once

#include <algorithm>
#include <cmath>
#include <decimal/decimal>
#include <ostream>
#include <string>


//#include "types.hpp"
#include "reusable.hpp"

std::decimal::decimal128 dbl_to_dec(double d, int dp);
std::decimal::decimal128 str_to_dec(const std::string& s, int dp);
std::decimal::decimal128 str_to_dec(double sgn, const std::string& s, int dp);

template<int WIDTH, int DP>
class decn 
{ 
	typedef decn<WIDTH, DP> decn_t;
	public:
		decn() {};
		decn(long before, long after)
		{
		       	dec = std::decimal::make_decimal128((long long)(before* pow(10, DP) + after), -DP); 
		}

		decn(double d) { double d1 = bround(d * pow(10, DP)); 
			dec = std::decimal::make_decimal128((long long)d1, -DP); };
		decn(const std::string& s) { dec = str_to_dec(s, DP); };
		std::decimal::decimal128 dec ;
		int width = WIDTH;
	       	int dp = DP; 

		bool operator==(const decn_t& other) { return this->dec == other.dec; };		
		bool operator!=(const decn_t& other) { return this->dec != other.dec; };
		std::string pos_str() const { double d = std::decimal::decimal_to_double(dec); return format_num(fabs(d), WIDTH, DP); };
		std::string str() const { double d = std::decimal::decimal_to_double(dec); return format_num(d, WIDTH, DP); };
		std::string stra() const { double d = std::decimal::decimal_to_double(dec); return format_num(d, DP); };
		void from_str(double sgn, const std::string& s) { dec = str_to_dec(sgn, s, DP); } ;
		void from_str(const std::string& s) { dec = str_to_dec(s, DP); } ;
		friend decn_t operator+(decn_t lhs, const decn_t& rhs) { lhs.dec += rhs.dec; return lhs; };
		friend decn_t operator-(decn_t lhs, const decn_t& rhs) { lhs.dec -= rhs.dec; return lhs; };
		double dbl() const { return std::decimal::decimal_to_double(dec); } ;
		decn_t& operator+=(const decn_t& rhs) { this->dec += rhs.dec;} ;
		bool zerop() const { return dbl() == 0.0; };
		void negate() { dec = -dec; } ;

		friend std::ostream& operator<<(std::ostream& os, const decn_t& obj)
		{
			os << obj.str();
			return os;
		};

};

//std::ostream& operator<<(std::ostream& os, const dec& obj);

typedef decn<12, 2> currency;
typedef decn<12, 3> quantity;


class price
{ 
	public:
		int DP = 5; // "ostensibly"
		std::decimal::decimal128 the_price;		
		price() { the_price = 0 ; } ; //std::decimal::
		price(std::decimal::decimal128 dec) { the_price = dec;};
		price(const std::string& s) { from_str(s); };
		/*
		double operator=(price rhs) { 
			return std::decimal::decimal_to_double(rhs.the_price);
	       	};
		*/
		price& operator=(const price& rhs) { 
			//std::swap(the_price, other.the_price);
			the_price = rhs.the_price;
			return *this;
		};
		double dbl(const price& p) const { return std::decimal::decimal_to_double(p.the_price); } ;
		double dbl() const { return std::decimal::decimal_to_double(the_price); } ;
		std::string str() const { return format_num( std::decimal::decimal_to_double(the_price), 12, DP); };
		std::string stra() const { return format_num( std::decimal::decimal_to_double(the_price), DP); };
		friend price operator/(price lhs, const price& rhs) { return lhs.the_price / rhs.the_price; } ;
		friend price operator-(price lhs, const price& rhs) { return lhs.the_price - rhs.the_price; } ;
		void from_str(const std::string& s) {
		       	the_price = str_to_dec(s, DP); 
		};
		friend std::ostream& operator<<(std::ostream& os, 
				const price& obj)
		{
			os << obj.str();
			return os;
		}

};

price operator/(const currency& c, const quantity& q);
currency operator*(const price& p, const quantity& q);

std::string ret_curr(const currency& num, const currency& denom); 
