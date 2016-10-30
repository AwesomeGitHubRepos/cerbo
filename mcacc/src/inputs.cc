#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <stdlib.h>

#include "common.hpp"
#include "reusable.hpp"
#include "inputs.hpp"

using namespace std;

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


void insert_nacc(nacc_ts& ns, const strings& fields)
{	
	nacc_t n;
	n.acc = fields[2];
	n.alt = fields[3];
	n.typ = fields[4];
	n.scale = stod(fields[5]);
	n.desc = fields[5];
	ns[n.acc] = n;
}


void insert_comm(comm_ts& cs, const strings& fields)
{
	comm_t c;
	c.ticker = fields[2];
	c.down =fields[3];
	c.typ =fields[4];
	c.unit = fields[5];
	c.desc = fields[6];
	cs[c.ticker] = c;
}

etran_t mketran(const strings& fields)
{
	etran_t e;
	e.taxable = fields[8] == "T";
	e.dstamp = fields[1];
	e.sgn = fields[7] =="B"? 1 : -1;
	e.buy = e.sgn == 1;
	e.folio = fields[2];
	e.ticker = fields[3];
	e.qty.from_str(e.sgn, fields[5]);
	e.cost.from_str(e.sgn, fields[6]);
	e.typ = regular;
	return e;
}

etran_t mkleak_1(const strings& fields)
{
	etran_t e = mketran(fields);
	e.buy = false;
	e.typ = leak;
	return e;
}

ntran_t mkntran(const strings& fields)
{
	ntran_t n;
	n.dstamp=fields[1];
	n.dr=fields[2];
	n.cr=fields[3];
	n.amount.from_str(fields[6]);
	n.desc=fields[9];
	return n;
}

void insert_LVL03(inputs_t& inputs, const strings& fields)
{
	string subtype = fields[4];
	if(subtype == "ETRAN-1") { 
		inputs.etrans.push_back(mketran(fields));
	} else if (subtype == "LEAK-1") {
		inputs.etrans.push_back(mkleak_1(fields));
	} else if (subtype == "NTRAN-1") {
		inputs.ntrans.push_back(mkntran(fields));
	} else {
		cerr << "inputs.cc:insert_LVL03() couldn't understand type ";
		cerr << subtype << ". Fatal exit." << endl;
		exit(EXIT_FAILURE);
	}
}

yahoo_t make_yahoo(inputs_t& inputs, const strings& fields)
{
	yahoo_t y;
	y.dstamp = fields[2];
	y.tstamp = fields[3];
	y.ticker = fields[4];
	//y.rox = stod(fields[5]);
	y.yprice.from_str(fields[6]);
	y.chg.from_str(fields[7]);
	y.chgpc = stod(fields[8]);
	y.currency = fields[9];
	y.desc = fields[10];
	return y;

}


void insert_yahoo(const yahoo_t& y, inputs_t& inputs)
{
	inputs.yahoos.insert(y);
}


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

void set_period(inputs_t& inputs, const strings& fields)
{
	inputs.p.start_date = fields[2];
	inputs.p.end_date = fields[3];
}

inputs_t read_inputs()
{
	inputs_t inputs;

	string fname;
	s1("derive-2.txt", fname);
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
				insert_nacc(inputs.naccs, row);
				break;
			case 2: // LVL02
				insert_comm(inputs.comms, row);
				break;
			case 3: // LVL03
				insert_LVL03(inputs, row);
				break;
			case 4: // LVL04
				set_period(inputs, row);
				break;
			case 5: // LVL05
				insert_LVL05(inputs, row);
				break;
			default:
				cerr << "Unhandled level number " << level << " in inputs.cc/read_inputs()\n";
				exit(EXIT_FAILURE);
		}
	}

	//assert(has_ticker(inputs.yahoos, "KCOM.L"));
	return inputs;

}
