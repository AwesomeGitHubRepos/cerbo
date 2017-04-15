#include "cgt.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <cmath>
#include <set>
#include <string>

#include "common.hpp"
#include "dec.hpp"
//#include "cpq.hpp"
#include <supo_general.hpp>

using namespace supo;

using std::endl;
using std::ofstream;
using std::string;

string mkrow(const etran_c& e)
{

	//const string US = string((char) 31);
	constexpr char US = (char) 31;
	string y = e.dstamp.substr(0, 4);
	string m = e.dstamp.substr(5, 2);
	string d = e.dstamp.substr(8, 2);
	string dstamp = d + "/" + m + "/" + y;

	//string share_str = e.qty.pos_str();
	
	//price p = e.cost/e.qty;
	//string price_str = p.str();

	string q = e.qty.str();
	string c = e.cost.str();

	//return  intercalate("\t", {e.buystr(), dstamp, e.ticker, share_str, 	
	//		price_str, "0.00", "0.00"});
	return  intercalate(US, {e.buystr(), dstamp, e.ticker, q, c});
}


void cgt(const etran_cs& es, const period &per)
{
	// TODO can file opening like this be refactored into a common lib?
	string fname = s3("cgt-1.rep");
	ofstream sout;
	sout.open(fname);

	std::set<string> tickers;
	for(auto& e: es)
		if(e.taxable && per.during(e.dstamp)  && ! e.buy)
			tickers.insert(e.ticker);

	for(const auto& e:es)
		if(tickers.find(e.ticker) != end(tickers))
			sout << mkrow(e) << "\n";

	constexpr char FS = (char) 28; //file separator
	sout << FS << "\n";
	sout.close();

}
