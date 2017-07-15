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
#include <supo_general.hpp>
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
		//string t1 = (format("%1%\t*\tetran\n") % e.dstamp).str() ;
		string t1 = e.dstamp + "\t*\tetran\n";
		assert(e.typ != unknown);
		string at = (e.typ == leak) ? "@" : "@@";
		//string lcost = format_num(labs(e.cost), 2);
		string t2 = "\tEquity:" + e.folio 
			+ "\t\"" + e.ticker + "\""
			+ "\t" + e.qty.str()
			+ "\t" + at 
			+ "\tGBP\t" + abs(e.cost).str(); 
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


	string dat = "N\tGBP\n\n";
	for(auto& p: trans) {
		dat += p.second;
	}

	return dat;
	//string fname = rootdir() + "/ledger.dat";
	//spit(fname, dat);
}


/*
void spit_strings(const string& filename, const multiset<string>& lines)
{
	ofstream out;
	out.open(filename, ofstream::trunc);
	for(auto& line: lines) out << line << endl;
	out.close();

}
*/

// create the prices
string mkprices(const yahoo_ts&  ys)
{
	multiset<string> prices;
	for(const auto& y: ys) {
		string price_str = format_num(y.yprice.dbl()/100, 7);
		string ticker = "\"" + y.ticker + "\"";
		strings fields = {"P", y.dstamp, y.tstamp, ticker, 
			"GBP", price_str};
		string line = intercalate("\t", fields);			
		prices.insert(line);
	}

	string result;
	for(const auto& p:prices) result += p + "\n";

	return result;
	//cout << result;
	//string fname = rootdir() + "/prices.dat";
	//spit_strings(fname, prices);
}

void wiegley(const etran_cs& etrans, const ntran_ts& ntrans, const yahoo_ts& yahoos)
{
	// note that I split this out into two functions for profiling purposes
	const string ledger = mkledger(etrans, ntrans);
	const string prices = mkprices(yahoos);
	string fname = rootdir() + "/ledger.dat";
	spit(fname, ledger+prices);
}
