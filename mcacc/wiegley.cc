#include <algorithm>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <set>
#include <utility>
#include <math.h>
#include <string>
#include <vector>

#include "common.h"
#include <supo_general.h>
#include "yahoo.h"
#include "reports.h"

using namespace std;
using namespace supo;

typedef pair<string, string> spair;

bool sorter(spair a, spair b)
{
	return a.first < b.first;
}

// create the transactions
string mkledger(const etran_cs& es, const ntran_ts& ns)
{
	vector<spair> trans;

	for(auto& e: es) {
		string t1 = e.dstamp + "\t*\tetran\n";
		assert(e.typ != unknown);
		string at = (e.typ == leak) ? "@" : "@@";
		string t2 = "\tEquity:" + e.folio 
			+ "\t\"" + e.ticker + "\""
			+ "\t" + e.qty.str()
			+ "\t" + at 
			+ "\tGBP\t" + std::to_string(abs(e.cost())); 
		string t3 = "\n\t" + e.folio + "\n\n";
		//string t3 = "\n\n";
		string t = t1 + t2 + t3;
		trans.push_back(make_pair(e.dstamp, t));
	}

	for(auto& n: ns) {
		string t1 = n.dstamp + "\t*\t" + n.desc + "\n";
		string amt = n.amount.str();
		string t2 = "\t" + n.dr + "\tGBP\t" + amt + "\n";
		string t3 = "\t" + n.cr + "\n\n";
		string t = t1 + t2 + t3;
		trans.push_back(make_pair(n.dstamp, t));
	}

	sort(begin(trans), end(trans), sorter);


	string dat = R"(
N	GBP

account	Balance:Msa
	alias	msa
	
account	Balance:Shares
	alias	Equity

account Balance:Short:Hfax
	alias	hal

account Balance:Short:Hargreaves
	alias	hl

account	Balance:Short:IGG
	alias	igg

account	Balance:Short:Rbd
	alias	rbd

account	Balance:Short:Rbs
	alias	rbs

account	Balance:Short:Tdi
	alias	tdi

account PaL:Income:Dividends
        alias	div

account	PaL:Income:Interest
	alias	int

account PaL:Income:Wages
        alias   wag

account PaL:Expenses:Amazon
        alias   amz

account	PaL:Expenses:Car
	alias	car

account PaL:Expenses:Charity
        alias   chr

account	PaL:Expenses:Computer
	alias	cmp

account	PaL:Expenses:Holiday
	alias	hol

account	PaL:Expenses:ISP
	alias	isp

account	PaL:Expenses:Misc
	alias	msc

account	PaL:Expenses:Mum
	alias	mum

account	PaL:Expenses:Taxes
	alias	tax


)";

	for(auto& p: trans) {
		dat += p.second;
	}

	return dat;
}


// create the prices
string mkprices(const yahoo_ts&  ys)
{
	multiset<string> prices;
	for(const auto& y: ys) {
		string price_str = format_num(y.yprice()/100, 7);
		string ticker = "\"" + y.ticker + "\"";
		strings fields = {"P", y.dstamp, y.tstamp, ticker, 
			"GBP", price_str};
		string line = intercalate("\t", fields);			
		prices.insert(line);
	}

	string result;
	for(const auto& p:prices) result += p + "\n";

	return result;
}

void wiegley(const etran_cs& etrans, const ntran_ts& ntrans, const yahoo_ts& yahoos)
{
	// note that I split this out into two functions for profiling purposes
	const string ledger = mkledger(etrans, ntrans);
	const string prices = mkprices(yahoos);
	string fname = rootdir() + "/ledger.dat";
	spit(fname, ledger+prices);
}
