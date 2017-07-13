#include <set>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <functional>
#include <stdexcept>

#include "common.h"
#include "types.h"
#include "posts.h"
#include <supo_general.hpp>

using namespace std;
using namespace supo;

void etb_main(nacc_ts& the_naccs, const post_ts& posts)
{

	//reset the account balances
	for(auto& n:the_naccs) n.second.bal = 0;

	std::string fname;
	int total;

	ofstream aout, eout;
	fname = s3("accs.rep");
	aout.open(fname);
	fname = s3("etb.dsv");
	eout.open(fname);

	set<string> keys;
	for(const auto& p:posts) {keys.insert(p.dr); }

	for(const auto& k: keys) {
		currency total;
		for(const auto& p:posts){
			if(p.dr != k) continue;
			// normal case
			aout << std::left;
			aout << setw(6) << p.dr;
			aout << setw(11) << p.dstamp;
			aout << setw(6) << p.cr;
			string desc = p.desc;
			desc.resize(30, ' ');
			//desc = pad_right(desc, 30);
			aout << setw(30) << desc;
			//write_centis(aout, p.amount);
			aout << p.amount.str();
			total += p.amount;
			//total.write(aout);
			aout << total.str();
			aout << endl;
		}

		aout << endl;

		if(the_naccs.count(k) == 0) {
			string msg =  "etb_main() couldn't look up key "s + k + " in naccs";
			throw std::out_of_range(msg);
		}

		nacc_t& a_nacc = the_naccs.at(k);

		double scale = a_nacc.scale;
		eout << pad_right(k, 6) << " "; 
		//write_centis(eout, total);
		eout << total.str() ;
		eout << " " << format_num(scale, 2, 0) << " " << format_num(total.dbl(), 10, 0);
		eout << endl;
		a_nacc.bal = total;

	}
	eout.close();
	aout.close();

}
