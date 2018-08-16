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

#include <supo.h>

#include "args.h"
#include "common.h"
//#include "oven.h"
#include "tests.h"
#include <supo_general.h>
#include "show.h"
//#include "scanner.h"
#include "parser.hh"


using namespace std;
using namespace supo;

// http://www.linuxquestions.org/questions/programming-9/deleting-a-directory-using-c-in-linux-248696/
// remove directory recursively
// include dirent.h sys/types.h
int rmdir(const char *dirname)
{
    DIR *dir;
    struct dirent *entry;
    char path[PATH_MAX];

    if (path == NULL) {
        fprintf(stderr, "Out of memory error\n");
        return 0;
    }
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("Error opendir()");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
            snprintf(path, (size_t) PATH_MAX, "%s/%s", dirname, entry->d_name);
            if (entry->d_type == DT_DIR) {
                rmdir(path);
            }

	    remove(path);
        }

    }
    closedir(dir);
    remove(dirname);

    return 1;
}

int rmdir(const string& dirname)
{
	return rmdir(dirname.c_str());
}

void clean()
{
	/*
	rmdir(sndir(1));
	rmdir(sndir(2));
	rmdir(sndir(3));
	*/
	for(int i=1; i<=3; ++i) rmdir(sndir(i));
}

//////////////////////////////////////////////////////////////////////////////////////
// section mcarter 16-Aug-2018

static string start_date{"0"};

std::map<string, double> bals;

void ntran_1(string dstamp, string dr, string cr, string amount, string desc, double sgn)
{
	double amnt = sgn * stod(amount);
	auto inc = [](string acc, double by) { 
		bals.insert( std::pair<string, double>(acc, by)); 
		bals[acc] += by;
	};

	if(dstamp < start_date) return;
	inc(dr, amnt);
	inc(cr, -amnt);
	
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
	etran() || ntran() || start() ;
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

void print_balances()
{
	//for(auto it = bals.;begin(); it != bals.end(); ++it) {
	for(auto b : bals) {
		cout << b.first << " " << b.second << "\n";
	}

}

// section mcarter 16-Aug-2018
//////////////////////////////////////////////////////////////////////////////////////

int 
main(int argc, char *argv[])
{
	scan("accts2018v1.txt");
	scan("ltbh.txt");
	scan("gaap.txt");
	print_balances();
	//scan("test.txt");
	cout << "TODO\n";
	return 0;

	feenableexcept(FE_OVERFLOW);
	const vm_t vm = parse_args(argc, argv);

	if(vm.count("clean")) clean();

	if(vm.count("pre")>0) {
		string pre = vm.at("pre");
		//preprocess(pre.c_str());
	}

	string wiegley_str = vm.at("wiegley");
	bool do_wiegley = wiegley_str == "on";
	if(! do_wiegley && wiegley_str != "off") {
		cerr << "ERR: Option wiegley error. Must be on|off, but given`"
			<< wiegley_str 
			<< "'. Continuing anyway." << endl;
	}

	/*
	oven ove;
	ove.m_vm = vm;
	ove.load_inputs();
	

	do_wiegley = true; // override defaults and just do it anyway
	bool do_fetch = vm.at("snap") == "on";
	ove.process(do_wiegley, do_fetch);

	//supo::ssystem("mcacc-reports.sh", true);
	if(vm.count("show") > 0) show(vm.at("show"));
	*/

	return EXIT_SUCCESS;
}
