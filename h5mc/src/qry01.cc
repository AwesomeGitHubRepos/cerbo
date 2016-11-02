/*
 * print the relative 5y strength of each sector,
 * ignoring companies on AIM.
 *
 * The point is to identify the worst-performing sectors,
 * and then invest in them.
 */

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>

#include <supo_stats.hpp>

#include "common.hpp"

using namespace std;


const string root = "/home/mcarter/.fortran/STATSLIST";

// TODO put to a reuasable
void rectify()
{
	string fnout = root + "/StatsList-1.csv";
	ofstream ofs(fnout.c_str(), std::ofstream::out);
	string raw = slurp(root + "/StatsList.csv");
	strings rows = split(raw, 13); // ^M
	strings::iterator it = begin(rows);

	string hdr_raw = *it;
	string hdr;
	for(int i=0; i< hdr_raw.size(); i++) { 
		if(hdr_raw[i] == 'F' && hdr_raw[i+1] == '.') i+=2;
		hdr += hdr_raw[i];
	}
	ofs << hdr << endl;

	while(++it != end(rows)) {
		string str = *it; // contains trailing ,
		ofs << str.substr(0, str.size()-1) << endl;
	}
	ofs.close();
}


// reusable
std::string dout(double d)
{
	std::ostringstream s;
	s.precision(2);
	s.width(7);
	s << std::fixed;
	s <<  d;
	return s.str();
}


typedef struct { 
	double m5; 
	double m1 ; 
	string sector;
	//result(double a_m5, double b, string s
} result;

/*
bool operator<<(const result& l, const result& r)
{
	return l.m5 < r.m5;
}
*/

bool rsorter(const result& lhs, const result& rhs)
{
	return lhs.m5 > rhs.m5;
}




void db(double n) 
{ 
	if(false)
		cout << n << endl;
}

int main()
{
	db(0);
	rectify();

	db(1);
	coldata cd;
	cd.read();
	cd.write_rec();

	strings indices = cd.get_strings("FTSE_Index");
	cells rs5s = cd.column["RS_5Year"];
	cells rs1s = cd.column["RS_Year"];
	strings sects = cd.get_strings("F.Sector");
	if(rs5s.size() == 0 || rs5s.size() != rs1s.size() 
			|| rs5s.size() != sects.size())
	{
		cerr << "Size mismatch\n";
		return(EXIT_FAILURE);
	}

	set<string> sectors(sects.begin(), sects.end());
	//vector<string> outvec;
	vector<result> outvec;
	db(3);
	for(const auto&s: sectors) {
		doubles ds1, ds5;
		for(int i=0; i<indices.size(); ++i) {
			if(s != sects[i]) continue;
			if(indices[i] == "AIM") continue;
			//double rs5 = boost::get<double>(rs5s[i]);
			db(3.1);
			double rs5 = rs5s[i].getd();
			db(3.2);
			if(isnan(rs5)) continue;
			//cout << rs5 << endl;
			ds5.push_back(rs5);
			db(3.3);
			ds1.push_back(rs1s[i].getd());
			db(3.4);
		}
		db(4);
		result r {supo::median(ds5), supo::median(ds1), s};
		outvec.push_back(r);

	}

	sort(begin(outvec), end(outvec), rsorter);
	for(const auto& line:outvec)
		cout << dout(line.m5) 
			<< " " << dout(line.m1)
			<< " " << line.sector << endl;

	return 0;
}
