//#include <decimal/decimal>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <assert.h>
#include <string>
#include <vector>
#include <set>

#include "common.h"
#include "dec.h"
#include <supo_general.h>
#include "stend.h"
#include "epics.h"
#include "types.h"

using namespace std;
//using namespace std::decimal;
using namespace supo;

        

detran_cs folio_c::filter(const detran_cs& es) const
{
	detran_cs result;
	for(const auto& e:es){
		const string& folio = e.folio;
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
string ticker(const detran_c& e) { return e.ticker; }
currency ecost(const detran_c& e) { return e.cost;}
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
	if(e.ticker == "AZN.L") n_azn++;
	if(n_azn>1) return;
	if(e.folio != "hal") return;

	cout << pad_ticker(e.ticker) << e.vto.str() << " ";
	cout << e.qty.str() << e.end_price.str();
       cout << vto.str() << endl;
}


void print_index(const string& index, const stend& s, ostream& pout)
{
	const price& sp = s.start_price;
	const price&  ep = s.end_price;
	const price chg = sub(ep,sp);
	string rstr = ret_str(ep, sp);
	pout << pad_ticker(index)
		<< as_currency(sp)
		<< nchars(' ', 11)
		<< as_currency(chg)
		<< as_currency(ep)
		<< rstr
		<< endl;
}
void print_indices(const stend_ts& stends, ostream &pout)
{
	pout << endl;
	string fname;

	auto indices = strings {"^FTSE", "^FTMC", "^FTAS"};
	for(string& i:indices) {
		try {
			const stend s = stends.at(i);
			print_index(i, s, pout);
		}
		catch(const std::out_of_range& oor) {
			cerr << "WARN: No stend for " + i << endl;
		} 
	}
}

currency sum(const detran_cs& es, std::function<currency (detran_c)> f)
{
	currency amount;
	for(const auto& e:es) {
		amount += f(e);
		//cout << e.etran.ticker << " " << f(e) << " " << amount << endl;
	}
	//cout << endl;
	return amount;
}

void folio_c::calculate(const detran_cs& all_etrans)
{
	const detran_cs by_epics = reduce(filter(all_etrans));

	for(const auto& esum:by_epics){
		if(esum.qty.zerop()) { 
			zeros.insert(esum.ticker);
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

	string hdr = "TICKER          QTY        COST       VALUE   RET%  "s +
		"     UCOST      UVALUE"s;
	ofs << hdr << endl;

	for(const auto& e:reduced_epics)
	{
		ofs << pad_ticker(e.ticker)
			<< e.qty.wide()
			<< e.cost.wide()
			<< e.vto.wide()
			<< ret_curr(e.vto, e.cost)
			<< e.ucost.wide()
			<< e.end_price.wide()
			<< endl;
	}

	ofs << pad_ticker("Grand:") << nchars(' ', 12) 
		<< cost.wide() << value.wide()
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
	const string rstr = ret_str(pdp+vbefore, vbefore);
	ofs << pad_right(m_name, 6) 
		<< vbefore.wide() << flow.wide()
	       	<< pdp.wide() << value.wide() << rstr << endl;
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
	const string hdr = 
		"FOLIO      VBEFORE       VFLOW     VPROFIT         VTO   VRET";
	pout << hdr << endl;
	for(const auto& f:folios)
		f.print_to_portfolio_file(pout);
	print_indices(stends, pout);
	pout.close();

	return folios;
}
