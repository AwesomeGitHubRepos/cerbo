#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "common.hpp"
#include "inputs.hpp"
#include "reusable.hpp"
#include "posts.hpp"

using namespace std;

bool operator<(const post_t& a, const post_t& b)
{
	return std::tie(a.dr, a.dstamp) < std::tie(b.dr, b.dstamp);
}

/*
void push(post_ts &ps, const string& dstamp, const string& ticker, 
		const string& acc, string str, double sgn, 
		const currency& amount)
{
	post_t p;
	if(amount.zerop()) return;
	p.dstamp = dstamp;
	p.dr = acc;
	p.cr = str;
	p.amount = amount;
	if(sgn == -1) p.amount.negate();
	p.desc = str + ":" + ticker;
	ps.push_back(p);
}
*/

void push_fpost(post_ts& ps, const string& acc, const string& desc,
		double sgn, const currency& amount)
{
	post_t p;
	if(amount.zerop()) return;
	p.dstamp = "3000-TODO";
	p.dr = acc;
	p.cr = "NOOP";
	p.amount = amount;
	if(sgn == -1) p.amount.negate();
	p.desc = desc;
	ps.push_back(p);
}

//post_ts posts_main(const inputs_t& inputs, const folio_cs& folios)
post_ts posts_main(const nacc_ts& naccs, const ntran_ts& ntrans, 
		const folio_cs& folios, const period& perd)
{

	post_ts ps;
	
	for(auto& n:ntrans) {
		if(n.dstamp > perd.end_date) continue;
		if(n.amount.zerop()) continue;

		post_t p;
		p.dstamp = n.dstamp;
		p.dr = n.dr;
		p.cr = n.cr;
		if(n.dstamp < perd.start_date) {
			p.dr = naccs.at(p.dr).alt;
			p.cr = naccs.at(p.cr).alt;
		}
		p.amount = n.amount;
		p.desc = n.desc;
		ps.push_back(p);

		std::swap(p.dr, p.cr);
		p.amount.negate();
		ps.push_back(p);
	}
	

	for(const auto& f:folios) {
		if(f.m_name == "total" || f.m_name == "mine") continue;
		/*
		auto push1 = [=](const string& affix, const string& desc,
				double sgn, const currency& amount){
			push(ps, "3000-TODO", "TBD1", f.m_name+affix,
					desc, sgn, amount);
		};
		push1("", "pCost", -1, currency(666));
		//push(ps, "3000-TODO", "TBD1", f.m_name , "pCost", f.pdp);
		//push(ps, "3000-TODO", "TBD1", f.m_name + "_g", "gain", f.pdp);
		*/
		//push_fpost(ps, f.m_name       , "pCost",  1, f.cost);
		push_fpost(ps, f.m_name + "_g", "pPdp",  -1, f.pdp);
		push_fpost(ps, "opn",           "pPbd",  -1, f.pbd);
		push_fpost(ps, f.m_name + "_c", "pPcd",   1, f.pdp + f.pbd);
	}

	/*
	for(const auto& ae: augetrans) {
		const etran_t& e = ae.etran;
		if(e.dstamp > inputs.p.end_date) continue;

		push(ps, e.dstamp, e.ticker, e.folio,        "pCost", 
				-1, e.cost);
		push(ps, e.dstamp, e.ticker, e.folio + "_c", "pVcd", 
				1, ae.vto);
		push(ps, e.dstamp, e.ticker, e.folio + "_g", "pPdp",
			       -1, ae.profit);
		push(ps, e.dstamp, e.ticker, "opn",          "pbd", 
				-1, ae.prior_year_profit);
	}
	*/

	sort(begin(ps), end(ps));
	return ps;
}
