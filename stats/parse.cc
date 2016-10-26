/* separate out inputs into dsv files
created 19-Feb-2016
*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>
//#include <fstream>
//#include <iostream>
//#include <math.h>
//#include <memory>

#include "parse.h"

//using namespace std;


const char white[] = " \t\r";

std::string trim(std::string& str)
{
    if(str.length() ==0) { return str;}
    size_t first = str.find_first_not_of(white);
    if(first == std::string::npos) return "";
    size_t last = str.find_last_not_of(white);
    return str.substr(first, (last-first+1));
}

std::vector<std::string> tokenize_line(std::string &input) //, const char sep= ' ')
{
	std::vector<std::string> output;
	std::string trimmed = input;
	std::string token;
	size_t first;
	int i;
loop:
	trimmed = trim(trimmed);
	if(trimmed.length() == 0) goto fin;
	//cout << "len = " << trimmed.length() << endl;
	if(trimmed[0] == '#') goto fin;
	if(trimmed[0] == '"') {
		token = "";
		for(i=1; i<trimmed.length(); i ++) {
			switch (trimmed[i]) {
			case '"':
				output.push_back(token);
				trimmed = trimmed.substr(i+1);
				goto loop;
			case ' ':
				token += '_' ; //167 ; // 26; // so that fields work
				break;
			default :
				token += trimmed[i];
			}
		}
		// hit eol without enclosing "
		output.push_back(token);
		goto fin;
	} else { // normal case
		first = trimmed.find_first_of(white);
		if(first == std::string::npos) {
			output.push_back(trimmed);
			goto fin;
		}
		token = trimmed.substr(0, first);
		output.push_back(token);
		trimmed = trimmed.substr(first+1);
		goto loop;
	}
	assert(false); // we should never get here


fin:
	return output;
}

/*
void get_period(string &start, string &end)
{
	ifstream fin;
	fin.open("s1/period.dsv", ifstream::in);
	string line;

	start = "0000-01-01";
	end   = "3000-12-31";
	while(getline(fin, line)) {
		vector<string> fields = tokenize_line(line);
		start = fields[0];
		end = fields[1];
	}

	//cerr << start << " " << end << endl ;
}



vecvec_t vecvec(string  &filename)
{
	vecvec_t res;
	ifstream fin;
	string line;
	fin.open(filename.c_str(), ifstream::in);
	while(getline(fin, line)) {
		vector<string> fields = tokenize_line(line);
		if(fields.size() >0) res.push_back(fields);
	}
	return res;
}

//void prin_vecvec(vecvec_t & vvs, const char *sep = "\t", const char *recsep= "\n", const char *filename = "")
void prin_vecvec(vecvec_t & vvs, const char *sep, const char *recsep, const char *filename )
{
	if(strlen(filename)>0) freopen(filename, "w", stdout);
	
	string ssep = string(sep);
	//vecvec_t::iterator it1;
	int i;
	//for(it1 = vv.begin() ; it1 != vv.end(); ++it1) {
	for(i=0; i< vvs.size(); i++) {
	//cout << "." ;
		//vector<string> v = *it1;
		//vector<string>::iterator it2;
		vector<string> v = vvs[i];
		int j, len;
		len = v.size();
		if(len == 0) continue;
		//if(it2.size() ==0) continue;
		for(j=0; j<len; j++) {
			//cout << "!";
			cout << v[j];
			if(j+1<len) cout << ssep;
		//for(it2 = v.begin(); it2 != v.end(); ++it2) {
		//	fout << *it2 << sep;
		}
		//if(len>0) cout << endl ;
		if(len>0) cout << recsep ;
	}

	//if(strlen(filename)>0) fout.close();
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

*/
