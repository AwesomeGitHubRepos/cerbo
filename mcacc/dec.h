#pragma once

#include <algorithm>
#include <cmath>
//#include <decimal/decimal>
#include <ostream>
#include <string>

#include <supo_general.hpp>

class price;
class quantity;

class currency {
	public:
		int value = 0;
		currency() {}
		currency(int i) { value = i;}
		currency(int whole, int frac);
		double operator() () const;
		std::string str() const;
		std::string wide() const;
		currency operator+=(const currency& rhs)
		{
			this->value = this->value + rhs.value;
			return *this;
		}
		friend currency operator-(currency lhs, const currency& rhs)
		{
			lhs.value -= rhs.value;
			return lhs;
		}
		friend currency operator+(currency lhs, const currency& rhs)
		{ 
			lhs.value += rhs.value;
			return lhs;
		}
		currency& operator=(const currency& other)
		{
			if(this != &other) {
				this->value = other.value;
			}
			return *this;
		}
		currency operator=(int i)
		{
			return currency(i);
		}
		void from_str(double sgn, const std::string& s) { 
			value = sgn * std::stod(s) * 100.0;
		} ;
		void from_str(const std::string& s) { 
			from_str(1, s);
		} ;
		std::string stra() const;
		bool zerop() const { return value == 0; }
		void negate() { value = - value; }
		bool operator==(const currency& rhs)
		{
			return this->value == rhs.value;
		}
		bool operator!=(const currency& rhs)
		{
			return this->value != rhs.value;
		}
		friend std::ostream& operator<<(std::ostream& os, const currency& obj)
		{
			os << obj.str();
			return os;
		}

};


currency operator*(const price& p, const quantity& q);
currency operator*(const currency& c, const price& p);
price operator/(const currency& c, const quantity& p);

class price {
	public:
		double value = 0;
		price() {}
		price(double p) { value = p;}
		price(std::string s) { value = std::stod(s); }
		std::string str() const;
		double operator() () const { return value; }
		void from_str(const std::string& s); 
		std::string stra();
		friend std::ostream& operator<<(std::ostream& os, const price& obj)
		{
			os << obj.str();
			return os;
		}
		
		friend price operator/(const currency& c, const quantity& q);

};

class quantity {
	public:
		double value = 0;
		quantity() {}
		quantity(std::string s) { value = std::stod(s);}
		double operator() () const { return value; }
		double get() const { return value; }
		std::string str() const;
		bool zerop() const { return value == 0.0; }
		quantity operator+=(const quantity& rhs)
		{
			this->value = this->value + rhs.value;
			return *this;
		}
		void from_str(double sgn, const std::string& s) { 
			value = sgn * std::stod(s);
			//dec = str_to_dec(sgn, s, DP); 
		} ;
		quantity(int whole, int frac);
		friend std::ostream& operator<<(std::ostream& os, const quantity& obj)
		{
			os << obj.str();
			return os;
		}
};

std::string as_currency(const price& p);

price div(const currency& c, const quantity& q);
price mul(const currency& c, const quantity& q);
currency mul(const price& p, const quantity& q);
price sub(const price& p1, const price& p2);
std::string ret_curr(const currency& num, const currency& denom); 
#if 0
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

		decn(double d) { 
			double d1 = supo::bround(d * pow(10, DP)); 
			dec = std::decimal::make_decimal128((long long)d1, -DP); };
		decn(const std::string& s) { dec = str_to_dec(s, DP); };
		std::decimal::decimal128 dec ;
		int width = WIDTH;
	       	int dp = DP; 

		bool operator==(const decn_t& other) { return this->dec == other.dec; };		
		bool operator!=(const decn_t& other) { return this->dec != other.dec; };
		std::string pos_str() const { 
			double d = std::decimal::decimal_to_double(dec); 
			return supo::format_num(fabs(d), WIDTH, DP); 
		};
		std::string str() const { 
			double d = std::decimal::decimal_to_double(dec); 
			return supo::format_num(d, WIDTH, DP); 
		};
		std::string stra() const { 
			double d = std::decimal::decimal_to_double(dec); 
			return supo::format_num(d, DP); };
		void from_str(double sgn, const std::string& s) { 
			dec = str_to_dec(sgn, s, DP); 
		} ;
		void from_str(const std::string& s) { dec = str_to_dec(s, DP); } ;
		friend decn_t operator+(decn_t lhs, const decn_t& rhs) { lhs.dec += rhs.dec; return lhs; };
		friend decn_t operator-(decn_t lhs, const decn_t& rhs) { lhs.dec -= rhs.dec; return lhs; };
		double dbl() const { return std::decimal::decimal_to_double(dec); } ;
		bool zerop() const { return dbl() == 0.0; };
		void negate() { dec = -dec; } ;
		friend decn_t abs(decn_t lhs) { lhs.dec = lhs.dec >= 0? lhs.dec : -lhs.dec; return lhs; };


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
		price& operator=(const price& rhs) { 
			//std::swap(the_price, other.the_price);
			the_price = rhs.the_price;
			return *this;
		};
		double dbl(const price& p) const { return std::decimal::decimal_to_double(p.the_price); } ;
		double dbl() const { return std::decimal::decimal_to_double(the_price); } ;
		std::string str() const {
		       	return supo::format_num( std::decimal::decimal_to_double(the_price), 12, DP); };
		std::string stra() const { 
			return supo::format_num( std::decimal::decimal_to_double(the_price), DP); };
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

#endif
