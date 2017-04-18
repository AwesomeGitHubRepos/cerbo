#include <getopt.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

//#include "shlex.hpp"
//using namespace shlex;

#include "supo_parse.hpp"
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::getline;
using std::string;
using std::vector;

typedef vector<string> strings;

const string help_text = R"hlp(
Options:
  -h [ --help ]    produce help mesage
  -v [ --version ] version
)hlp";

void
process()
{
	string fs = "\t";
	string rs = "\n";

	string line;
	while(getline(cin, line))
	{
		strings vals = supo::tokenize_line(line);
		for(int i=0; i< vals.size(); ++i) {
			cout << vals[i];
			if( i < vals.size()-1)
				cout << fs;
			else
				cout << rs;
		}
	}
}

int 
main(int argc, const char *argv[])
{
	//options opts;
        int c;
        while(1) {
                int this_option_optind = optind ? optind : 1;
                int option_index = 0;
                static struct option long_options[] = {
                        {"help", no_argument, 0, 'h'},
			{"m4", required_argument, 0, '4'},
                        {"version", no_argument, 0, 'v'},
                        {0 ,0,0,0}
                };
                c = getopt_long(argc, (char* const*)argv, "hf:v",
                                long_options,  &option_index);
                if(c == -1) break;

                switch(c) {
                        case 'h': cout << help_text; exit(EXIT_SUCCESS);
                        //case '4': opts.ofmt = M4; break;
                        case 'v': std::cout << "shlex (" << PACKAGE_NAME << ") " << VERSION << "\n"; exit(EXIT_FAILURE);
                        default:  cerr << "getopt returned character code " << c << endl; exit(EXIT_FAILURE);
                }
        }
        if (optind < argc) {
                printf("non-option ARGV-elements: ");
                while (optind < argc)
                        printf("%s ", argv[optind++]);
                printf("\n");
        }


	//shlex::shlexmat mat = shlex::read(std::cin, opts);
	//shlex::write(mat, opts);
	process();
	return EXIT_SUCCESS;
}
