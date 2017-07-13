#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <stdexcept>

#include "common.h"
#include "inputs.h"
#include <supo_general.hpp>
#include "posts.h"

using namespace std;
using namespace supo;

bool operator<(const post_t& a, const post_t& b)
{
	return std::tie(a.dr, a.dstamp) < std::tie(b.dr, b.dstamp);
}


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

post_ts posts_main(const inputs_t& inputs, 
		const folio_cs& folios, const period& perd)
{

	ntran_ts ns = inputs.ntrans;	
	for(const auto& e: inputs.etrans) {
		ntran_t n;
		n.dstamp = e.dstamp;

		n.dr = e.folio + "_c"; 
		n.cr = e.folio ;

		n.amount = e.cost;
		n.desc = "pCost:" + e.ticker;

		ns.push_back(n);
	}

	post_ts ps;
	for(auto& n:ns) {
		if(n.dstamp > perd.end_date) continue;
		if(n.amount.zerop()) continue;

		post_t p;
		p.dstamp = n.dstamp;
		p.dr = n.dr;
		p.cr = n.cr;
		if(n.dstamp < perd.start_date) {
			p.dr = inputs.naccs.at(p.dr).alt;
			p.cr = inputs.naccs.at(p.cr).alt;
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
		push_fpost(ps, f.m_name + "_g", "pPdp",  -1, f.pdp);
		push_fpost(ps, "opn",           "pPbd",  -1, f.pbd);
		push_fpost(ps, f.m_name + "_c", "pPcd",   1, f.pdp + f.pbd);
	}


	map<string, currency> opening_balances;
	post_ts ps1;
	for(const auto& p: ps) {
		if(p.dstamp < perd.start_date) {
			auto it = opening_balances.find(p.dr);
			if(it == end(opening_balances)) opening_balances[p.dr] = 0;
			opening_balances[p.dr] += p.amount;
		} else {
			ps1.push_back(p);
		}
	}
	//puts("TODO opening_balances");
	for(auto b: opening_balances){
		if(b.second == 0) continue;
		post_t p;
		p.dstamp = perd.start_date;
		p.dr = b.first;
		p.cr = "NOOP";
		p.amount = b.second;
		p.desc = "Opening balance";
		ps1.push_back(p);
		//cout << b.first << "\t" << b.second << endl;
	}

	sort(begin(ps1), end(ps1));
	return ps1;
}
