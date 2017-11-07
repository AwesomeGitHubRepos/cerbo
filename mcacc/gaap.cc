/*
 * 18-Jun-2016 Can't use autosprintf on cygwin
 * */

#include <cmath>
#include <map>
#include <iostream>
#include <fstream>
#include <locale.h>
#include <string.h>

#include "common.h"
#include <supo_general.h>
#include "gaap.h"



using namespace std;
using namespace supo;

const string rmargin = "  ";

// TODO put as part of generalisation
strings operator+(strings lhs, const strings& rhs)
{
	for(string r:rhs) lhs.push_back(r);
	return lhs;
}

strings operator+(strings lhs, const string& rhs)
{
	lhs.push_back(rhs);
	return lhs;
}

strings& operator+=(strings& a, string b)
{
	a.push_back(b);
	return a;
}

nacc_ts m_naccs;

currency get_bal(string key)
{ 
	try {
		return m_naccs.at(key).bal;
	} catch (const std::out_of_range& ex) {
		return currency();
	}	

}	


// TODO I'm sure we can do better than this:
class section {
	public:
		struct Lie { 
			string desc; 
			currency amount; 
		}; // line entry


		strings the_lines;
		void enline(string line) { the_lines += (line + rmargin) ; } ;
		void underline1() { enline("           ---------- "); };
		void blank()      { enline("                     "); };


		section(string desc) {
			total_lie.desc = desc;
		}

		section add(string desc) {
			currency amount = get_bal(desc);
			struct Lie lie = {desc, amount};
			add(lie);
			return *this;
		}

		section add(section s) {
			struct Lie lie {s.total_lie.desc, s.total_lie.amount};
			add(lie);
			return *this;
		}

		section add(Lie lie) {
			lies.push_back(lie);
			total_lie.amount += lie.amount;
			enline(get_lie(lie));
			return *this;
		}

		section total1() {
			underline1();
			enline(get_lie(total_lie));
			underline1();
			blank();
			return *this;
		}

		section adds(vector<string> descs) {
			for(auto& d:descs) add(d);
			return *this;
		}

		section running(string desc) {
			struct Lie lie = total_lie;
			lie.desc = desc;
			underline1();
			lies.push_back(lie);
			enline(get_lie(lie));
			return *this;
		}


		
		strings lines() {
			return the_lines;
		}
	

		string emit(const string& title, const currency& value);
		string get_lie(const struct Lie& lie);



	private:
		struct Lie total_lie; 
		vector<Lie> lies;
};

string section::emit(const string& title, const currency& value)
{
	double bal = round2(value());
	char sgn = bal<0 ? '-' : ' ';
	bal = fabs(bal);
	setlocale(LC_NUMERIC, "");
	char buf[80];
	snprintf(buf, sizeof(buf), "%10.10s %'10.2f%c", title.c_str() , bal, sgn);
	//(*ofs) << buf << endl;
	return buf;


}

string section::get_lie(const struct Lie& lie)
{
	return emit(lie.desc, lie.amount);
}


strings period_hdr(const period& p)
{
	string s1 = pad_right("START", 11)  + p.start_date + rmargin + " ";
	string s2 = pad_right("END", 11)  + p.end_date + rmargin + " ";
	return strings {s1, s2, ""};
}

void write_gaap_files(const strings& lines)
{
	string fname;

	fname = s3("gaap-1.rep");
	ofstream gout1;
	gout1.open(fname.c_str(), ofstream::out);

	fname = s3("gaap-2.rep");
	ofstream gout2;
	gout2.open(fname.c_str(), ofstream::out);

	string fname_in = s0("gaap-past.rep");	
	ifstream ifs;
	ifs.open(fname_in, ifstream::in);

	for(const auto& line:lines)
	{
		string comparative;
		getline(ifs, comparative);
		gout1 << line << endl;
		gout2 << line << comparative << endl ;
	}

	gout1.close();
	gout2.close();
	ifs.close();
}


void gaap_main(const nacc_ts& the_naccs, const period& per)
{
	m_naccs = the_naccs;

	auto get_bal = [the_naccs](auto key){ 
		currency bal;
		try {
			bal = the_naccs.at(key).bal;
		} catch (const std::out_of_range& ex) {
			//bal = 0;
		}	
		return bal;
	};	



	section inco = section("Income").adds({"div", "int", "wag"}).total1();
	section exps = section("Expenses") .adds( {"amz", "car", "chr", 
			"cmp", "hol", "isp", "msc", "mum"}).total1();
	section mygains = section("Folio Gain").adds( {"hal_g", "hl_g", 
			"igg_g", "tdi_g", "tdn_g"}).total1();
	section balcd = section("Bal c/d").add(inco).add(exps)
		.running("Ord profit")
		.add("tax").add(mygains).add("ut_g").running("Net Profit")
		.add("opn").total1();
	section folio = section("Folio")
		.adds( {"hal_c", "hl_c", "igg_c", "tdi_c", "tdn_c"})
		.running("My shares")
		.add("ut_c").total1();

	section nass = section("Net Assets")
		.adds( {"hal", "hl", "igg", "ut", "rbs", "rbd", "sus", 
				"tdi", "tdn", "tds", "vis"})
		.running("Cash equiv")
		.add(folio)
		.add("msa").total1();

	strings lines = period_hdr(per) + balcd.lines() + nass.lines() 
		+ inco.lines() + exps.lines() + mygains.lines() 
		+ folio.lines();


	write_gaap_files(lines);
}
