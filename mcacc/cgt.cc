#include "cgt.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <cmath>
#include <set>
#include <string>
#include <vector>

#include "common.h"
#include "dec.h"
#include <supo_general.hpp>
#include <supo_parse.hpp>

using namespace supo;

using std::endl;
using std::ofstream;
using std::string;
using std::vector;

string mkrow(const etran_c& e)
{

	constexpr char US = (char) 31;
	string y = e.dstamp.substr(0, 4);
	string m = e.dstamp.substr(5, 2);
	string d = e.dstamp.substr(8, 2);
	string dstamp = d + "/" + m + "/" + y;

	string q = std::to_string(fabs(e.qty.dbl())); //supo::trim(e.qty.str());
	auto uprice = (e.cost/e.qty).dbl();
	string ustr = std::to_string(fabs(uprice/100.0));
	strings outs = {e.buystr(), dstamp, e.ticker, q, ustr, "0.00", "0.00"};
	return intercalate("\t", outs);
}


void cgt(const etran_cs& es, const period &per)
{
	// TODO can file opening like this be refactored into a common lib?
	string fname = s3("cgt-1.rep");
	ofstream sout;
	sout.open(fname);

	std::set<string> tickers;
	for(auto& e: es) {
		bool taxable = e.folio != "tdi";
		if(taxable && per.during(e.dstamp)  && ! e.buy)
			tickers.insert(e.ticker);
	}

	for(const auto& e:es)
		if(tickers.find(e.ticker) != end(tickers))
			sout << mkrow(e) << "\n";

	sout.close();

}
