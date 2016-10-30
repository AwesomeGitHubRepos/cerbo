#include <algorithm>
#include <cassert>
#include <fstream>
#include <cmath>
#include <sstream>
#include <unistd.h>
#include <vector>

#include "reusable.hpp"

using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::string;
using std::vector;

bool file_exists(const string& filename)
{
	return ( access( filename.c_str(), F_OK ) != -1 );
}

string slurp(const char *filename)
{
	ifstream in;
	in.open(filename, ifstream::in);
	stringstream sstr;
	sstr << in.rdbuf();
	in.close();
    	return sstr.str();
}

std::string slurp(const std::string filename) { return slurp(filename.c_str());}

void spit(const char *filename, const char *content)
{
	ofstream out;
	out.open(filename, ofstream::trunc);
	out << content;
	out.close();

}


void spit(const std::string& filename, const std::string& content)
{ 
	spit(filename.c_str(), content.c_str());
}

// http://stackoverflow.com/questions/236129/split-a-string-in-c
void split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	stringstream ss;
	ss.str(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
}


std::vector<std::string> split(const std::string &s, char delim)
{
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}


/* remove all characters in-place */ 
void erase_all(std::string& str, const char c)
{
	for(auto it=str.begin(); it != str.end(); ++it)
		if(*it ==c) str.erase(it--);
}

string nchars(char c, int num) 
{
	string res = "";
	for(int i=0; i<num; i++) res += c;
	return res;
}
// pad right to n characters
string pad_right(string s, int n) { return s + nchars(' ', n - s.size()); }

string pad_left(string s, int n) { return nchars(' ', n-s.size()) + s; }

/** format a double N as a string to DP decimal places
 Includes required are #include <sstream> [sic]
 * */
string format_num(double n, int dp)
{
	std::ostringstream s;
	s.precision(dp);
	s << std::fixed;
	s <<  n;
	return s.str();
}

string format_num(double n, int width, int dp)
{
	std::ostringstream s;
	s.width(width);
	s.precision(dp);
	s << std::fixed;
	s <<  n;
	return s.str();
}

std::string intercalate(std::string inter, std::vector<std::string> strings)
{
	string res = "";
	for(auto it = strings.begin(); it != strings.end(); it++) {
		res += *it;
		if(it< strings.end()-1) res += inter;
	}
	return res;
}

// http://stackoverflow.com/questions/4891006/how-to-create-a-folder-in-the-home-directory
std::string expand_user(std::string path) 
{
	if (not path.empty() and path[0] == '~') {
		assert(path.size() == 1 or path[1] == '/');  // or other error handling
		char const* home = getenv("HOME");
		if (home or ((home = getenv("USERPROFILE")))) {
			path.replace(0, 1, home);
		}
		else {
			char const *hdrive = getenv("HOMEDRIVE"),
			     *hpath = getenv("HOMEPATH");
			assert(hdrive);  // or other error handling
			assert(hpath);
			path.replace(0, 1, std::string(hdrive) + hpath);
		}
	}
	return path;
}


double bround(double x) 
{
  double c ,f, det;
  c = ceil(x);
  f = floor(x);

  // eliminate strange artifacts of creating "negative 0"
  if(c == 0) c = 0.0;
  if(f == 0) f = 0.0;
  // printf("c: %f ", c);


  det = c + f - 2.0*x;
  if (det < 0) {return c;}
  if (det > 0) {return f;}

  /* banker's tie */
 if(2*ceil(c/2) == c) {return c;} else {return f;};

}

double round2(double x) { return bround(x*100)/100; }

