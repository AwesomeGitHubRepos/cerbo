#include <assert.h>
#include <cfenv>
#include <cmath>
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
#include <sstream>


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

string intercalate(const strings& strs, const string& sep)
{
	string result;
	int len = strs.size();
	for(int i=0; i<len; ++i) {
		result += strs[i];
		if(i+1<len) result += sep;
	}
	return result;
}

void tout(const strings& strs)
{
	cout << intercalate(strs, "\t") << "\n";
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
	double dqty = stod(qty);
	double sgn = dqty < 0? 1 : -1;
	ntran_1(dstamp, acc, "flow", amount, desc, sgn);
	tout({"qty-1", acc+":"+ticker, qty});

	// stocko output
	string dstamp1 = dstamp.substr(8,2) + "/" + dstamp.substr(5, 2) + "/" + dstamp.substr(0, 4);
	string typa {"Buy"}, typb{"Deposit"};
	double qamount = stod(amount);
	if(dqty<0) { typa = "Sell"; typb = "Withdrawal"; qamount *= -1; }
	string price = to_string(fabs(qamount * 100.0 /dqty));
	static char sconsid[30];
	sprintf(sconsid, "%.2f", qamount);
	auto sto = [&](string sym, string typ, string q, string u, string p, string rox, string r) { 
		cout << "stocko-1\t"; 
		strings fields = {sym, dstamp1, "10:10:10", typ, q, u, p, rox, r, "", sconsid};
		cout << intercalate(fields, ",") << "\n"; 
	};
	string ticker1 = "LON:"s + ticker.substr(0, ticker.size()-2);
	string aqty = to_string(int(abs(dqty)));
	sto(ticker1, typa, aqty,   "1",   price, "GBX", "0");
	sto("",   typb, "",  "", "", "", "");

	// cgt output
	string way = dqty >0 ? "B" : "S";
	string cprice = to_string(fabs(qamount/dqty));
	tout({"cgt-1", way, dstamp1, ticker, aqty, cprice, "0.00", "0.00"});

	return true;
}

bool fail()
{
	set<string> ignore = {"nb", "stocko-off"};
	if(ignore.find(command_name) != ignore.end())
		return true;
	cerr << "Unknown command:" << command_name << ".\n";
	return false;
}

bool gaap()
{
	if(command_name != "gaap") return false;
	strings& a = command_args;
	string meta{a.at(0)}, acc{a.at(1)}, cmp{a.at(2)}, desc{a.at(3)};
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
	command_name = s;
	command_args.clear();
}

void add_argument(std::string s)
{
	command_args.push_back(s);
}

void dispatch_command()
{
	etran() || ntran() || gaap() || start() || fail();
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
	tout({"stocko-1", "TICKER,DATE,TIME,TYPE,SHARES,FX,PRICE,CURRENCY,COMMISSION,TAX,TOTAL"});
	scan("accts2018v2.txt");
	scan("ltbhv2.txt");
	scan("gaap.txt");
	return 0;
}
