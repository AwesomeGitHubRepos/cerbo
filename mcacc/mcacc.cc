#include <assert.h>
#include <cfenv>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <string>
#include <vector>
#include <stdexcept>
#include <sys/stat.h>
#include <unistd.h>
#include <ostream>
#include <sstream>


#include "args.h"
#include "common.h"
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

//////////////////////////////////////////////////////////////////////////////////////
// section mcarter 16-Aug-2018

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
	inc_bal(cr, -amnt);
	
}

/////////

static string command_name;
strings command_args;

bool etran()
{
	if(command_name != "etran-2") return false;
	strings& a = command_args;
	//string dstamp{a.at(0)}, acc{a.at(1;
	string dstamp{a.at(0)}, acc{a.at(1)}, ticker{a.at(2)}, 
	       qty{a.at(3)}, amount{a.at(4)}, way{a.at(5)}, desc{a.at(6)};
	double sgn = way == "S"? 1 : -1;
	ntran_1(dstamp, acc, "flow", amount, desc, sgn);
	//std::tie(dstamp, acc, ticker, qty, amount, way, desc) = command_args;
	//ntran_1(a.at(0),  
	//cout << "I'm an etran\n";
	return true;
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
	etran() || ntran() || gaap() || start() ;
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
	scan("accts2018v1.txt");
	scan("ltbh.txt");
	scan("gaap.txt");
	//print_balances();
	//scan("test.txt");
	//cout << "TODO\n";
	return 0;
}
