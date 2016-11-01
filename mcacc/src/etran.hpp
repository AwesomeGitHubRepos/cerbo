#pragma once

#include <set>
#include <string>
//#include <vector>

#include "dec.hpp"
#include "types.hpp"

enum Etype { unknown, leak, regular };

class etran_c {
	public:

	bool		taxable = true;
	dstamp_t	dstamp;
	double 		sgn;
	bool		buy = true;
	std::string	folio;
	quantity	qty;
	currency  	cost;
	std::string	ticker = "<UNKNOWN>";
	Etype typ = unknown;
	std::string	buystr() const { return buy? "B" : "S"; };
	friend bool operator<(const etran_c& lhs, const etran_c& rhs){
		return std::tie(lhs.ticker, lhs.dstamp) 
			< std::tie(rhs.ticker, rhs.dstamp);
	};
};

class detran_c {
	public:
		etran_c etran;
		// derived fields:
		price		ucost; 
		dstamp_t	start_dstamp;
		price		start_price;
		dstamp_t	end_dstamp;
		price		end_price;
		currency	prior_year_profit;
		currency	vbefore;
		currency	flow;
		currency	profit;
		currency	vto;
		detran_c& operator+=(const detran_c& rhs);
		friend bool operator<(const detran_c& l, const detran_c& r)
		{
			return l.etran < r.etran;
		}

};


//bool operator<(const etran_t& lhs, const etran_t& rhs);

bool same_ticker(etran_c a, etran_c b);
//typedef std::vector<etran_t> etran_ts;
typedef std::multiset<etran_c> etran_cs;

typedef std::vector<detran_c> detran_cs;
