#include <getopt.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include <supo.hpp>

using namespace std;

string help_text=R"hlp(
Options:
  -h [ --help ]            produce help message
  -f [ --file ] arg (=mnu) file FILE as menu descriptor, instead of <mnu>
  -v [ --version ]         version
)hlp";


std::string slurp(const char *filename)
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	std::stringstream sstr;
	sstr << in.rdbuf();
	in.close();
	return sstr.str();
}

typedef struct mitem { string text; string cmd; } mitem;
typedef	vector<mitem> menu_t;

void read_cfg(menu_t& menu, const string& cfg_file)
{
	ifstream in;
	in.open(cfg_file, ifstream::in);

	menu.push_back( mitem {"EXIT", "noop"}) ;

	while(!in.eof()) {
		string text, cmd, dummy;
		getline(in, text);
		if(text == "") continue;
		getline(in, cmd);
		getline(in, dummy);
		menu.push_back( mitem {text, cmd});
	}

	in.close();
}

void print_menu(const menu_t& menu)
{
	for(int i=0; i< menu.size() ; ++i) {
		cout << " " << i << " " <<menu[i].text << endl;
	}
}

bool loop(const menu_t& menu)
{
	bool more = true;	
	print_menu(menu);
	cout << "Enter selection, followed by return\n";
	string sel;
	getline(cin, sel);
	int n;
	try { 
		n = stoi(sel);
	} catch (invalid_argument& ex)
	{
		cerr << "Invalid argument: <" << sel << ">\n\n";
	}

	if(n>menu.size())
	{
		cerr << "Not a valid number\n" ;
	}

	if(n==0) return false;

	supo::ssystem(menu[n].cmd.c_str(), true);

	return more;
}

int main(int argc, const char *argv[])
{
	// process args:
	// https://linux.die.net/man/3/getopt_long
	string cfg_file = "mnu";
	int c;
	while(1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{"help", no_argument, 0, 'h'},
			{"file", required_argument, 0, 'f'},
			{"version", no_argument, 0, 'v'},
			{0 ,0,0,0}
		};
		c = getopt_long(argc, (char* const*)argv, "hf:v",
				long_options,  &option_index);
		if(c == -1) break;

		switch(c) {
			case 'h': cout << help_text; exit(EXIT_SUCCESS);
			case 'f': cfg_file = optarg; break;
			case 'v': std::cout << "menu (" << PACKAGE_NAME << ") " << VERSION << "\n"; exit(EXIT_FAILURE);
			default:  cerr << "getopt returned character code " << c << endl; exit(EXIT_FAILURE);
		}
	}
	if (optind < argc) {
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}

	menu_t menu;
	read_cfg(menu, cfg_file);
	while(loop(menu));

	return EXIT_SUCCESS;
}
