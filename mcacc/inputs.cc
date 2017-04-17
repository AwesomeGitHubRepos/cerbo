#include <cassert>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <stdexcept>

#include "common.hpp"
#include <supo_parse.hpp>
#include "inputs.hpp"

using namespace std;
//using namespace supo;


typedef std::function<void(inputs_t&, const strings&)> infunc;

void insert_nacc(inputs_t& inputs, const strings& fields)
{	
	nacc_t n;
	constexpr int base = 0;
	n.acc = fields[base];
	n.alt = fields[base+1];
	n.typ = fields[base+2];
	n.scale = stod(fields[base+3]);
	n.desc = fields[base+4];
	nacc_ts& ns = inputs.naccs;
	ns[n.acc] = n;
}


//void insert_comm(comm_ts& cs, const strings& fields)
void insert_comm(inputs_t& inputs, const strings& fields)
{
	comm_t c;
	constexpr int base = 0;
	c.ticker = fields[base];
	c.down =fields[base+1];
	c.typ =fields[base+2];
	c.unit = fields[base+3];
	c.desc = fields[base+4];
	comm_ts& cs = inputs.comms;
	cs[c.ticker] = c;
}

etran_c mketran(const strings& fields)
{
	etran_c e;
	constexpr int base = 0;
	e.dstamp = fields[base];
	e.folio = fields[base+1];
	e.ticker = fields[base+2];
	
	e.sgn = fields[base+5] =="B"? 1 : -1;
	e.qty.from_str(e.sgn, fields[base+3]);
	e.cost.from_str(e.sgn, fields[base+4]);

	e.taxable = fields[base+6] == "T";
	e.buy = e.sgn == 1;
	e.typ = regular;
	return e;
}

void insert_etran_1(inputs_t& inputs, const strings& fields)
{
	const etran_c e = mketran(fields);
	inputs.etrans.insert(e);

	// insert a synthesised etran
	yahoo_t y;
	y.dstamp = e.dstamp;
	y.tstamp = "12:00:00";
	y.ticker = e.ticker;
	const price p = e.cost / e.qty;
	y.yprice = p;
	y.chg = price();
	y.chgpc = 0;
	y.currency = "P"; // TODO LOW
	y.desc = "synthasised from etran";
	inputs.yahoos.insert(y);

}

etran_c mkleak_1(const strings& fields)
{
	etran_c e;
	e.dstamp = fields[0];
	e.folio = fields[1];
	e.ticker = fields[2];
	e.sgn = -1;
	e.qty.from_str(e.sgn, fields[3]);
	// e.cost I think we can ignore
	e.taxable = fields[4] == "T";
	e.buy = false;
	e.typ = leak;
	return e;
}
void
insert_leak_1(inputs_t& inputs, const strings& fields)
{
	const etran_c e = mkleak_1(fields);
	inputs.etrans.insert(e);
}
ntran_t mkntran(const strings& fields)
{
	ntran_t n;
	constexpr int base = 0;
	n.dstamp=fields[base];
	n.dr=fields[base+1];
	n.cr=fields[base+2];
	n.amount.from_str(fields[base+3]);
	n.desc=fields[base+4];
	return n;
}

void
insert_ntran(inputs_t& inputs, const strings& fields)
{
	const ntran_t n = mkntran(fields);
	inputs.ntrans.push_back(n); 
}


void
insert_yahoo_1(inputs_t& inputs, const strings& fields)
{
	yahoo_t y;
	constexpr int base = 0;
	y.dstamp = fields[base];
	y.tstamp = fields[base+1];
	y.ticker = fields[base+2];
	//y.rox = stod(fields[5]);
	y.yprice.from_str(fields[base+4]);
	y.chg.from_str(fields[base+5]);
	y.chgpc = stod(fields[base+6]);
	y.currency = fields[base+7];
	y.desc = fields[base+8];
	inputs.yahoos.insert(y);
	//return y;

}


void 
set_period(inputs_t& inputs, const strings& fields)
{
	constexpr int base = 0;
	inputs.p.start_date = fields[base];
	inputs.p.end_date = fields[base+1];
}

void // do nothing!
skip(inputs_t& inputs, const strings& fields) {}

/*
void
read_csv_inputs(const string fname, 
		const int num_fields,
		infunc func,
		inputs_t& inputs)
{
	const string fname1 = s2(fname);
	ifstream ifs(fname1);
	string line;
	while(getline(ifs, line)) {
		stringstream ss(line);
		string cell;
		strings row;
		while(getline(ss, cell, ','))
			row.push_back(cell);
		if(num_fields != row.size()) {
			string msg = "Error in input:`" + line + "'.\n"
				+ "Expected " + to_string(num_fields)
			       	+ " fields, got "
				+ to_string(row.size());
			throw std::range_error(msg);

		}
		func(inputs, row);
		//cout << line << endl;
	}
	ifs.close();
}
*/



inputs_t read_inputs()
{
	inputs_t inputs;

	struct cmd_t {
		string name;
		int num_fields;
		infunc f;
		cmd_t(const string &in_name) : name(in_name) {};
		cmd_t(const string& in_name, int in_num_fields, const infunc& in_f) : name(in_name), num_fields(in_num_fields), f(in_f) {};
		bool operator<(const cmd_t &rhs) const { return name < rhs.name;};
		bool operator==(const cmd_t &rhs) const { return name == rhs.name;};
	};
	const std::set<cmd_t> cmds = {
		{"comm-1",  5, insert_comm},
		{"etran-1", 8, insert_etran_1},
		{"leak-1",  6, insert_leak_1},
		{"nacc",    5, insert_nacc},
		{"nb",     -1, skip},
		{"ntran",   5, insert_ntran},
		{"period",  2, set_period},
		{"yahoo-1", 9, insert_yahoo_1}
	};

	// TODO abstract and use wherever an input stream is used
	ifstream ifs(s1("derive-2.txt"));
	//supo::strmat mat = supo::tokenize_stream(ifs);
	string line;
	while(getline(ifs, line)){
		strings fields = supo::tokenize_line(line);
		if(fields.size() ==0) continue;
		auto search = cmds.find(fields[0]);
		if(search == cmds.end()){
			cerr << "ERR: Unrecognised command `" << fields[0] << "' in:" << line << "\n";
			continue;
		}
		fields.erase(begin(fields)); // the fields
		//assert(fields.size() == search->num_fields);
		search->f(inputs, fields); // do whatever is required for that field

	}


	/*
	for(auto& row: mat) {
		if(row.size() == 0) continue;
		//cout << row[0] << endl;
		auto search = cmds.find(row[0]);
		if(search == cmds.end()) continue;
		row.erase(begin(row)); // the fields
		assert(row.size() == search->num_fields);
		search->f(inputs, row); // do whatever is required for that field
	}
	*/

	ifs.close();
	return inputs;
}

