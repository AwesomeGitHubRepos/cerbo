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

#include <supo_stats.h>

#include "common.hpp"

using namespace std;


const string root = "/home/mcarter/.fortran/STATSLIST";


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

	db(1);
	coldata cd;
	cd.read();

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
