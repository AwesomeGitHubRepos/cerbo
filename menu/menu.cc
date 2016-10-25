#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include <boost/program_options.hpp>
namespace po = boost::program_options;



using namespace std;


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

	system(menu[n].cmd.c_str());

	return more;
}

int main(int argc, const char *argv[])
{
	po::options_description desc("Options");
	desc.add_options()
		("help,h", "produce help message")
		("file,f", po::value<string>()->default_value("mnu"), "file FILE as menu descriptor, instead of <mnu>")
		("version,v", "version")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if(vm.count("help")) {
		std::cout << desc << "\n";
		exit(EXIT_SUCCESS);
	}

	if(vm.count("version")) {
		std::cout << "shlex (" << PACKAGE_NAME << ") " << VERSION << "\n";
		exit(EXIT_SUCCESS);
	}

	string cfg_file = "mnu" ; //<< vm["f"].as<string>();
	if(vm.count("file")) cfg_file = vm["file"].as<string>();
	//cout << cfg_file <<endl;
	//exit(EXIT_FAILURE);

	menu_t menu;
	read_cfg(menu, cfg_file);
	while(loop(menu));



	return EXIT_SUCCESS;
}
