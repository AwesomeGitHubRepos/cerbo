#pragma once

#include <string>
#include <vector>

#include "dec.hpp"
#include "types.hpp"
//#include "quantity.hpp"
//#include "centis.hpp"
//#include "price.hpp"

enum Etype { unknown, leak, regular };

typedef struct etran_t {
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
} etran_t ;

class detran_c {
	public:
		etran_t etran;
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

};


bool operator<(const etran_t& lhs, const etran_t& rhs);

bool same_ticker(etran_t a, etran_t b);
typedef std::vector<etran_t> etran_ts;

typedef std::vector<detran_c> detran_cs;
