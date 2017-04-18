/* separate out inputs into dsv files
created 19-Feb-2016
*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
#include <memory>

#include "shlex.hpp"


namespace shlex {

typedef std::vector<std::string> strings;

	
const char white[] = " \t\r";

std::string trim(std::string& str)
{
    if(str.length() ==0) { return str;}
    size_t first = str.find_first_not_of(white);
    if(first == std::string::npos) return "";
    size_t last = str.find_last_not_of(white);
    return str.substr(first, (last-first+1));
}

std::vector<std::string> tokenize_line(const std::string &input, const options &opts)
{
	std::vector<std::string> output;
	std::string trimmed = input;
	std::string token;
	size_t first;
	int i;
loop:
	trimmed = trim(trimmed);
	if(trimmed.length() == 0) goto fin;
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
				token += opts.sep; // '_' ; //167 ; // 26; // so that fields work
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




shlexmat read(std::istream  &istr, const options& opts)
{
	shlexmat res;
	std::string line;
	while(getline(istr, line)) {
		std::vector<std::string> fields = tokenize_line(line, opts);
		if(fields.size() >0) res.push_back(fields);
	}
	return res;
}


shlexmat read(std::string  &filename, const options& opts)
{
	std::ifstream fin;
	fin.open(filename.c_str(), std::ifstream::in);
	shlexmat res  = read(fin, opts);
	fin.close();
	return res;
}


void prin_vecvec(const shlexmat & vvs, const char *sep, 
		const char *recsep, const char *filename )
{
	using namespace std::string_literals;
	if(strlen(filename)>0){
	       	FILE* fp = freopen(filename, "w", stdout);
		if(!fp) {
			std::string msg = "freopen() returned NULL on "s
				+ "filename <"s
				+ std::string(filename) +">"s;
			throw std::runtime_error(msg);
		}
	}
	
	std::string ssep = std::string(sep);
	int i;
	for(i=0; i< vvs.size(); i++) {
		std::vector<std::string> v = vvs[i];
		int j, len;
		len = v.size();
		if(len == 0) continue;
		for(j=0; j<len; j++) {
			std::cout << v[j];
			if(j+1<len) std::cout << ssep;
		}
		if(len>0) std::cout << recsep ;
	}

}


void write_m4(const strings& r)
{
	if(r.size() ==0) return;

	std::cout << r.at(0);
	if(r.size() > 1) {       
		std::cout << "(";
		for(auto it = std::begin(r)+1; it != std::end(r); ++it) {
			std::cout << "`" << *it << "'";
			if(it < std::end(r)-1) std::cout << ", ";
		}
		std::cout << ")";
	}
	std::cout << std::endl;
}

void write(const shlexmat &m, const options& opts) {
	switch(opts.ofmt) {
		case REG:
		       	prin_vecvec(m, "\n", "\n", ""); 
			break;
		case M4:
			for(auto& r:m) write_m4(r);
			break;
		default:
			std::string msg("write() did not handle all ofmt case: ");
			msg += opts.ofmt;
			throw std::logic_error(msg);
	}
}

shlexmat read(const char *fname, const options& opts)
{
	std::string fn = (fname);
	return read(fn, opts);
}


}

