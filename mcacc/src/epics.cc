#include <decimal/decimal>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <assert.h>
#include <string>
#include <vector>
#include <set>

#include "common.hpp"
#include "dec.hpp"
#include "reusable.hpp"
#include "stend.hpp"
#include "epics.hpp"

using namespace std;
using namespace std::decimal;

/*
void underline(ostream &ost, char c)
{ 
	auto fields = strings {pad_ticker(c), pad_gbp(c), 
		pad_gbp(c), pad_gbp(c), pad_gbp(c), ret_str(c) };
	print_strings(ost, fields);
}
*/

        

bool operator<(const detran_c& lhs, const detran_c& rhs)
{
	return lhs.etran < rhs.etran;
}

detran_cs folio_c::filter(const detran_cs& es) const
{
	detran_cs result;
	for(const auto& e:es){
		const string& folio = e.etran.folio;
		bool match = folio == m_name || 
			(m_name == "mine" && folio != "ut") ||
			m_name == "total";
		if(match)
			result.push_back(e);
	}
	std::sort(result.begin(), result.end()); // mostly for debugging convenience
	return result;
}

// TODO find better home, and use more often
string ticker(const detran_c& e) { return e.etran.ticker; }
currency ecost(const detran_c& e) { return e.etran.cost;}
currency evto(const detran_c& e) { return e.vto;}
currency epdp(const detran_c& e) { return e.profit;}
currency evbefore(const detran_c& e) { return e.vbefore;}
currency epbd(const detran_c& e) { return e.prior_year_profit;}



detran_c reduce_all (const detran_cs& es)
{
	detran_c result;
	for(const auto& e:es) result += e;
	return result;
}
detran_cs reduce(const detran_cs& es)
{
	detran_cs result;

	set<string> tickers;
       	for(const auto& e:es) tickers.insert(ticker(e));

	for(const auto& t:tickers) {
		detran_cs ticker_etrans;
		for(const auto& e:es) 
			if(ticker(e) == t) ticker_etrans.push_back(e);

		const detran_c reduction = reduce_all(ticker_etrans); 
		result.push_back(reduction);
	}
	return result;
}


void debug(const detran_c& e, const currency& vto)
{
	static int n_azn = 0;
	return; // just switch it off for now
	if(n_azn==0) cout << "epics.cc:debug() output follows\n";
	if(e.etran.ticker == "AZN.L") n_azn++;
	if(n_azn>1) return;
	if(e.etran.folio != "hal") return;

	cout << pad_right(e.etran.ticker, 6) << e.vto.str() << " ";
	cout << e.etran.qty.str() << e.end_price.str();
       cout << vto.str() << endl;
}


void print_indices(const stend_ts& stends, ostream &pout)
{
	pout << endl;
	string fname;

	auto indices = strings {"^FTSE", "^FTMC", "^FTAS"};
	for(string& i:indices){
		const stend s = stends.at(i, 
			"epic.cc:print_indices() couldn't find stend with key " + i);
		const price& sp = s.start_price;
		const price&  ep = s.end_price;
		const price chg = ep-sp;
		const double rat = ep.dbl()/sp.dbl();
		auto fields = strings { pad_ticker(i), 
			sp.str(), pad_gbp(' '), chg.str(), ep.str(), 
			ret_str(rat)};
		print_strings(pout, fields);
	}
}

currency sum(const detran_cs& es, std::function<currency (detran_c)> f)
{
	currency amount;
	for(const auto& e:es) amount += f(e);
	return amount;
}

void folio_c::calculate(const detran_cs& all_etrans)
{
	const detran_cs by_epics = reduce(filter(all_etrans));

	for(const auto& esum:by_epics){
		if(esum.etran.qty.zerop()) { 
			zeros.insert(esum.etran.ticker);
		} else {
			reduced_epics.push_back(esum);
		}
	}

	cost    = sum(by_epics, ecost); //esum.etran.cost;
	value   = sum(by_epics, evto); //esum.vto;
	pdp     = sum(by_epics, epdp);
	pbd     = sum(by_epics, epbd);
	vbefore = sum(by_epics, evbefore);
	flow    = value - pdp - vbefore;
	//cout << "pbd = " << pbd << endl;
}

void folio_c::print_to_epic_file(ofstream& ofs) const
{
	ofs << m_name << endl;

	string hdr = "TICKER        COST       VALUE   RET%         QTY"s +
		"       UCOST      UVALUE"s;
	ofs << hdr << endl;

	for(const auto& e:reduced_epics)
	{
		ofs << pad_left(e.etran.ticker, 6)
			<< e.etran.cost
			<< e.vto
			<< ret_curr(e.vto, e.etran.cost)
			<< e.etran.qty
			<< e.ucost
			<< e.end_price
			<< endl;
	}

	ofs << pad_right("Grand:", 6) << cost << value 
		<< ret_curr(value, cost) << endl << endl;

	if(m_name != "total") return;
	ofs << "Zeros:\n";
	int i=0;
	for(const auto& z:zeros) {
		ofs << pad_right(z, 6) << " ";
		i++;
		if(i==10) {i = 0; ofs << endl; }
	}
}

void folio_c::print_to_portfolio_file(ofstream& ofs) const
{
	if(m_name == "mine" || m_name == "total")
		ofs << nchars('-', 61) << endl;
	// TODO there is another function for this: use it
	const string rstr = ret_str((pdp+vbefore).dbl(), vbefore.dbl());
	ofs << pad_right(m_name, 6) << vbefore << flow
	       	<< pdp << value << rstr << endl;
		//"TODO folio_c::print_to_portfolio_file()\n";
	if(m_name == "total") ofs << nchars('=', 61) << endl;
}

folio_cs epics_main(const detran_cs& es, const stend_ts& stends)
{

	folio_cs folios = { 
		folio_c("hal"), folio_c("hl"), folio_c("igg"),
		folio_c("tdi"), folio_c("tdn"), folio_c("mine"), folio_c("ut"), 
		folio_c("total")
	};
	for(auto& f:folios)
		f.calculate(es);

	string filename = s3("epics.rep");
	ofstream eout;
	eout.open(filename);
	for(const auto& f:folios) f.print_to_epic_file(eout);
	eout.close();

	filename = s3("portfolios.rep");
	ofstream pout;
	pout.open(filename);
	/*
	strings fields = strings { pad_ticker("FOLIO"), pad_gbp("VBEFORE"), 
		pad_gbp("VFLOW"), pad_gbp("VPROFIT"), 
		pad_gbp("VTO"), ret_str("VRET")};
		*/
	const string hdr = 
		"FOLIO      VBEFORE       VFLOW     VPROFIT         VTO   VRET";
	//"hal       85336.11    27881.72   -11576.66   101641.17"
	//print_strings(cout, hdr);
	pout << hdr << endl;
	for(const auto& f:folios)
		f.print_to_portfolio_file(pout);
	print_indices(stends, pout);
	pout.close();

	return folios;
}
