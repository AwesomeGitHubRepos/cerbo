#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <typeinfo>
#include <unordered_map>

//#include "dec.h"
#include "inputs.h"
#include <supo_general.h>
#include "common.h"
#include "types.h"
#include "etran.h"

using namespace std;
using namespace supo;



detran_c augment(const etran_c& e, const stend_ts& stends, const period& per)
{
	detran_c aug;
	//aug = e;
	aug.etran_c::operator=(e);
	const stend& s = stends.at(e.ticker);

	//aug.ucost.reprice(e.cost, e.qty); 
	aug.ucost = div(e.cost, e.qty);
	aug.start_dstamp = s.start_dstamp;
	aug.start_price = s.start_price;
	aug.end_dstamp = s.end_dstamp;
	aug.end_price = s.end_price;

	quantity qty;
	qty += e.qty;
	aug.vbefore = 0;
	aug.flow = 0;
	aug.prior_year_profit = 0;
	aug.vto = mul(s.end_price, qty);
	switch(per.when(aug.dstamp)) {
		case perBefore:
			aug.vbefore = mul(s.start_price, qty);
			aug.prior_year_profit = aug.vbefore 
				- aug.cost;
			break;
		case perDuring:
			aug.flow = aug.cost;
			break;
		case perAfter:
			aug.vto = 0;
	}

	//aug.profit.set(aug.vto.get() - aug.vbefore.get() - aug.flow.get());
	aug.profit = aug.vto - aug.vbefore - aug.flow;

	return aug;
}

string asstr(const char& c) { return to_string(c); }
string asstr(const currency& c) { return c.str();}
string asstr(const string& s) { return s;}
string asstr(const price& p) { return p.str(); }
template <typename T>
void recline(ofstream& ofs, const string& fname, T const& fvalue)
{
	string fvs= asstr(fvalue);
	string fstr = pad_right(fname + ": ", 8);
	ofs << fstr << fvs << endl;
}

void write_augetran(ofstream& ofs, const detran_c& e)
{
	
	auto gout = [&ofs](const string& s, const auto& v) {
		recline(ofs, s, v);};

	//gout("Tax", e.etran.taxable? "T" : "F"); // 1
	gout("Dstamp", e.dstamp); // 2
	gout("Buy", e.buystr()); // 3
	gout("Folio", e.folio); // 4
	gout("Eticker", e.ticker); // 5
	gout("Qty", e.qty.str());
	gout("Cost", e.cost); // 7
	gout("Ucost", e.ucost); //dout(e.ucost, 6); // 8
	gout("Eticker", e.ticker); // 9
	gout("Start_dstamp", e.start_dstamp); // 10
	gout("Start_price", e.start_price); //dout(e.start_price, 6); // 11
	gout("End_dstamp", e.end_dstamp); // 12
	gout("End_price", e.end_price); //dout(e.end_price, 6); //13
	gout("Prior_year_profit", e.prior_year_profit); // 14
	gout("Vbefore", e.vbefore); // 15
	gout("Flow", e.flow); // 16
	gout("Profit", e.profit); // 17
	gout("Vto", e.vto); // 18
	ofs << endl;
}

detran_cs eaug_main(const etran_cs& etrans, const stend_ts& stends, 
		const period &per)
{
	detran_cs augs;

	for(auto& e:etrans) 
		if(e.dstamp <= per.end_date)
			augs.push_back(augment(e, stends, per));

	// save
	string fname = s3("etrans-aug.rec");
	ofstream ofs;
	ofs.open(fname);
	for(auto& e:augs) write_augetran(ofs, e);
	ofs.close();

	return augs;
}

detran_c& detran_c::operator+=(const detran_c& rhs){
	/*
	etran_c& elhs = this->etran;
	const etran_c& erhs = rhs.etran;
	elhs.ticker = erhs.ticker;
	*/

	if(this->buy){
		this->cost += rhs.cost;
	} else {
		this->cost += mul(div(this->cost,this->qty),  rhs.qty);
	}
	this->ticker = rhs.ticker;
	this->qty += rhs.qty;
	this->ucost = div(this->cost, this->qty);
	this->end_price = rhs.end_price;
	this->profit += rhs.profit;
	this->vto = mul(this->end_price,  this->qty);
	this->vbefore += rhs.vbefore;
	this->prior_year_profit += rhs.prior_year_profit;
	return *this;
}
