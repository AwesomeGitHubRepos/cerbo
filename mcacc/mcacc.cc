#include <assert.h>
#include <cfenv>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <string>
#include <vector>
#include <stdexcept>
//#include <sys/stat.h>
//#include <unistd.h>
//#include <ostream>
#include <sstream>


//#include "args.h"
//#include "common.h"
#include "parser.hh"


typedef std::vector<std::string> strings;

using namespace std;


std::string dout(double d)
{
        std::ostringstream s;
        s.precision(2);
        s.width(10);
        s << std::fixed;
        s <<  d;
        return s.str();
}

void tout(const strings& strs)
{
	int len = strs.size();
	for(int i=0; i<len; ++i) {
		cout << strs[i];
		if(i+1<len) cout << "\t";
	}
	cout << "\n";

}

//////////////////////////////////////////////////////////////////////////////////////

static string start_date{"0"};

std::map<string, double> bals;

void inc_bal(string acc, double by)
{
	bals.insert( std::pair<string, double>(acc, 0)); 
	bals[acc] += by;
}

double bals_at(string acc)
{
	bals.insert( std::pair<string, double>(acc, 0)); 
	return bals[acc];
}


void ntran_1(string dstamp, string dr, string cr, string amount, string desc, double sgn)
{

	if(dstamp < start_date) return;
	double amnt = sgn * stod(amount);

	//if(dr == "amz") cout << amnt << "\n";

	inc_bal(dr, amnt);
	tout({"post-1", dstamp, dr, dout(amnt), cr, desc});
	inc_bal(cr, -amnt);
	tout({"post-1", dstamp, cr, dout(-amnt), dr, desc});
	
}

/////////

static string command_name;
strings command_args;

bool etran()
{
	if(command_name != "etran-3") return false;
	strings& a = command_args;
	string dstamp{a.at(0)}, acc{a.at(1)}, ticker{a.at(2)}, 
	       qty{a.at(3)}, amount{a.at(4)}, desc{a.at(5)};
	double sgn = stod(qty) < 0? 1 : -1;
	ntran_1(dstamp, acc, "flow", amount, desc, sgn);
	tout({"qty-1", acc+":"+ticker, qty});
	return true;
}

bool fail()
{
	set<string> ignore = {"nb", "stocko-off"};
	if(ignore.find(command_name) != ignore.end())
		return true;
	//if(find(ignore.begin(), ignore.end(), command_name) != ignore.end())
	//	return true;
	//if(ignore.find(command_name) != ignore.end()) return true;
	cerr << "Unknown command:" << command_name << ".\n";
	return false;
}

bool gaap()
{
	if(command_name != "gaap") return false;
	strings& a = command_args;
	string meta{a.at(0)}, acc{a.at(1)}, cmp{a.at(2)}, desc{a.at(3)};
	//cout << "acc = " << acc << "\n";
	inc_bal(meta, bals_at(acc));
	string acc1 = acc == "=" ? meta : acc;
	double amount = bals_at(acc1);
	cout << "gaap-1\t" << acc1 << "\t" << dout(amount) << "\t" << dout(stod(cmp)) << "\t" << desc << "\n";
	if(acc == "=") cout << "gaap-1\n";
	return true;
}
bool ntran()
{
	if(command_name != "ntran") return false;
	ntran_1(command_args.at(0), command_args.at(1), command_args.at(2), 
			command_args.at(3), command_args.at(4), 1);
	//cout << "I'm an ntran\n";
	return true;
}

bool start()
{
	if(command_name != "start") return false;
	start_date = command_args.at(0);
	return true;
}


void start_command(std::string s)
{
	//cout << "start_command:" << s << "\n";
	command_name = s;
	command_args.clear();
}

void add_argument(std::string s)
{
	//cout << "add_arg:" << s << "\n";
	command_args.push_back(s);
}

void dispatch_command()
{
	//cout << "dispatch\n";
	//auto is = [command_name](string s) { return s == command_name; };
	//auto is = [](string s) { return s == command_name; };
	etran() || ntran() || gaap() || start() || fail();
	//if(is("etran-2"))
	//	cout << "found etran\n";
}

void scan(const char* path)
{
	string full{"/home/mcarter/repos/redact/docs/accts2018/"};
	full += path;
	freopen(full.c_str(), "r", stdin);
	yyparse();
}

//////////
// section mcarter 16-Aug-2018
//////////////////////////////////////////////////////////////////////////////////////

int 
main(int argc, char *argv[])
{
	feenableexcept(FE_OVERFLOW);
	scan("accts2018v2.txt");
	scan("ltbhv2.txt");
	scan("gaap.txt");
	//print_balances();
	//scan("test.txt");
	//cout << "TODO\n";
	return 0;
}
