#pragma once

#include <map>
#include <ostream>
#include <string>
#include <vector>

//#include "boost/variant.hpp"
std::string format(double d, int dp);

using std::map;
using std::string;
using std::vector;

class cell {
	public:
		cell(const string& s) { set(s);};
		cell(double d) {set(d);};
		void set(const string& newstring) { m_doublep = false; s = newstring;};
		void set(double& newdouble) { m_doublep = false; d = newdouble;};
		double getd() const { // TODO check that double= is true
			return d;};
		std::string gets() const { return s;};
		bool doublep() const { return m_doublep;};
	private:
		bool m_doublep = true;
		double d = 0;
		std::string s = "";
};

std::ostream& operator<<(std::ostream& os, const cell& obj);
//typedef boost::variant<double, string> cell;
typedef vector<cell> cells;

typedef vector<double> doubles;
typedef vector<string> strings;
typedef vector<strings> strmat ;

typedef struct col_s { string name ; bool is_num; int strlen=0; strings strs; vector<double> ds ; } col_s;

bool is_num(string &s);

string slurp(std::ifstream& in);
string slurp(const char * filename);
string slurp(const string &filename);
string trim(const string& str, const string &junk);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
void read_csv(vector<col_s> &cvecs);



class coldata {
	public:
		coldata() {};
		void read();
		void write_rec();
		//int num_rows = 0;
		doubles get_doubles(string colname, double scale);
		map<string, cells> column;
		strings get_strings(string colname);
};
