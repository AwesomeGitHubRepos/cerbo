#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

//#include "price.hpp"
#include "dec.hpp"

typedef struct yahoo_t {
	std::string dstamp;
	std::string tstamp;
	std::string ticker;
	//double rox = 1;
	price yprice;
	price chg;
	double chgpc = 0;
	std::string currency = "P";
	std::string desc = "no desc";
} yahoo_t;

bool operator<(const yahoo_t& a, const yahoo_t& b);

typedef std::set<yahoo_t> yahoo_ts;

std::vector<std::string> fetch_tickers(const std::vector<std::string>& tickers,
	       	std::string& usd);
bool has_ticker(const yahoo_ts& yahoos, const std::string& ticker);
