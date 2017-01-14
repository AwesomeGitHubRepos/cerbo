#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <stdexcept>

#include "common.hpp"
#include <supo_general.hpp>
#include "inputs.hpp"

using namespace std;
using namespace supo;

namespace parse {

typedef struct lexer_t {
	std::string::iterator cursor, end, tok_start, tok_end;
	std::string token() { 
		std::string s ; // = "";
		for(auto& it= tok_start; it != tok_end; it++) s+= *it;
		return s;
	}
	bool is_white() {
		char c = *cursor;
		return c == ' ' || c == '\t' || c == '\r';
	}

	bool more() { 
		if(cursor == end) return false;

		// eat white
		while(cursor != end) {
			if(! is_white()) break;
			cursor++;
		}
		if(*cursor == '#') return false;
		if(cursor == end) return false;

		tok_start = cursor;
		bool is_string  = *cursor == '"';
		if(is_string) {
			tok_start++;
			cursor++;
			while(cursor !=end && *cursor != '"') cursor++;
			tok_end = cursor;
			if(cursor !=end) cursor++;
		} else {
			while(!is_white() && cursor !=end) cursor++;
			tok_end = cursor;
		}

		return true;
	}
	lexer_t(std::string& s) {cursor = s.begin(); end = s.end(); }
} lexer_t;

std::vector<std::string> tokenise_line(std::string& s)
{
	std::vector<std::string> result;
	lexer_t lexer(s);

	while(lexer.more()) {
		std::string token = lexer.token();
		result.push_back(token);
	}
	return result;
}

vecvec_t vecvec(istream  &istr)
{
	vecvec_t res;
	string line;
	while(getline(istr, line)) {
		vector<string> fields = parse::tokenise_line(line);
		if(fields.size() >0) res.push_back(fields);
	}
	return res;
}

vecvec_t vecvec(const std::string& filename)
{
	ifstream fin;
	fin.open(filename.c_str(), ifstream::in);
	auto res  = parse::vecvec(fin);
	fin.close();
	return res;
}



void prin_vecvec(vecvec_t & vvs, const char *sep, const char *recsep, const char *filename )
{
	
	std::ofstream ofs;
	bool use_file = strlen(filename);
	if(use_file) ofs.open(filename, std::ofstream::out);
	ostream &os = use_file ? ofs : cout ;

	string ssep = string(sep);
	int i;
	for(i=0; i< vvs.size(); i++) {
		vector<string> v = vvs[i];
		int j, len;
		len = v.size();
		if(len == 0) continue;
		for(j=0; j<len; j++) {
			os << v[j];
			if(j+1<len) os << ssep;
		}
		if(len>0) os << recsep ;
	}

	if(use_file) ofs.close();
}


void prin_vecvec1(vecvec_t &vv)
{
	prin_vecvec(vv, "\n", "\n", "");
}
vecvec_t vecvec(const char *fname)
{
	string fn = (fname);
	return vecvec(fn);
}

} // namespace parse


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

	// TODO NOW insert an ntran
}

etran_c mkleak_1(const strings& fields)
{
	etran_c e;
	//	= mketran(fields);
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
	inputs.ntrans.push_back(n); // TODO HIGH - correct insertion order?
}

/*
void insert_LVL03(inputs_t& inputs, const strings& fields)
{
	string subtype = fields[4];
	if(subtype == "ETRAN-1") { 
		inputs.etrans.insert(mketran(fields));
	} else if (subtype == "LEAK-1") {
		inputs.etrans.insert(mkleak_1(fields));
	} else if (subtype == "NTRAN-1") {
		inputs.ntrans.push_back(mkntran(fields));
	} else {
		cerr << "inputs.cc:insert_LVL03() couldn't understand type ";
		cerr << subtype << ". Fatal exit." << endl;
		exit(EXIT_FAILURE);
	}
}
*/

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

/*
void 
insert_yahoo(const yahoo_t& y, inputs_t& inputs)
{
	inputs.yahoos.insert(y);
}
*/

/*
void insert_LVL05(inputs_t& inputs, const strings& fields)
{
	string subtype = fields[1];
	yahoo_t y = make_yahoo(inputs, fields);	
	if(subtype == "PRICE-1") {
		currency c(fields[6]);
		quantity q(fields[5]);
		y.yprice = c/q;
		//y.rox =1;
	} else if (subtype != "YAHOO-1") {
		cerr << "inputs.cc:insert_LVL05() couldn't understand type ";
		cerr << subtype << ". Fatal exit." << endl;
		exit(EXIT_FAILURE);
	}

	insert_yahoo(y, inputs);
}
*/

void set_period(inputs_t& inputs, const strings& fields)
{
	constexpr int base = 0;
	inputs.p.start_date = fields[base];
	inputs.p.end_date = fields[base+1];
}

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
void
read_inputs_1(inputs_t& inputs)
{
	//string fname = s2("comm-1.csv");

	struct csv_t {
		string fname;
		int num_fields;
		infunc f;
	};

	const vector<csv_t> csvs = {
		{"comm-1.csv", 5, insert_comm},
		{"etran-1.csv", 8, insert_etran_1},
		{"leak-1.csv", 6, insert_leak_1},
		{"nacc.csv",   5, insert_nacc},
		{"ntran.csv", 5, insert_ntran},
		{"period.csv", 2, set_period},
		{"yahoo-1.csv", 9, insert_yahoo_1}
	};

	for(const auto& c: csvs)
		read_csv_inputs(c.fname, c.num_fields, c.f, inputs);

}

// TODO deprecate
inputs_t read_inputs()
{
	inputs_t inputs;
/*
	string fname;
	s1("derive-1.txt", fname);
	vecvec_t mat = parse::vecvec(fname);

	for(auto& row:mat) {
		string cmd = row[0]; // => LVL0?
		string level_str = cmd.substr(3, 2);
		int level =stoi(level_str);
		switch(level) {
			case 0: //LVL00
				// these are just notes, so we can ignore them
				break;
			case 1: // LVL01
				// superceded by read_inputs_1()
				//insert_nacc(inputs.naccs, row);
				break;
			case 2: // LVL02
				// superceded by read_inputs_1()
				//insert_comm(inputs.comms, row);
				break;
			case 3: // LVL03
				// superceded by read_inputs_1()
				//insert_LVL03(inputs, row);
				break;
			case 4: // LVL04
				// superceded by read_inputs_1()
				//set_period(inputs, row);
				break;
			case 5: // LVL05
				// superceded by read_inputs_1()
				//insert_LVL05(inputs, row);
				break;
			default:
				cerr << "Unhandled level number " << level << " in inputs.cc/read_inputs()\n";
				exit(EXIT_FAILURE);
		}
	}

	//assert(has_ticker(inputs.yahoos, "KCOM.L"));
	*/
	read_inputs_1(inputs);
	return inputs;

}

