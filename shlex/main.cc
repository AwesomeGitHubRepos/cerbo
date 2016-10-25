#include <getopt.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>

// http://theboostcpplibraries.com/boost.program_options
//#include <boost/program_options.hpp>
//namespace po = boost::program_options;

#include "shlex.hpp"
using namespace shlex;

using std::cout;
using std::cerr;
using std::endl;
using std::string;

const string help_text = R"hlp(
Options:
  -h [ --help ]    produce help mesage
  -4 [ --m4 ]      output in m4 format
  -v [ --version ] version
)hlp";

/*
void process_args(int argc, const char *argv[], options &opts)
{
	po::options_description desc("Options");
	desc.add_options()
		("help,h", "produce help message")
		("m4", "output in m4 format")
		("version,v", "version");
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

	if(vm.count("m4")) opts.ofmt = M4;

}
*/

int main(int argc, const char *argv[])
{
        // process args:
        // https://linux.die.net/man/3/getopt_long
	options opts;
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
                        case '4': opts.ofmt = M4; break;
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

	//po::options_description desc("Options");
	/*
	try { process_args(argc, argv, opts); }
	catch (const std::exception& ex)
	{
		std::cerr << "Error processings args. Aborting abnormally.\nTry shlex -h\n";
		std::cerr << ex.what() << "\n";
		exit(EXIT_FAILURE);
	}
	*/

	shlex::shlexmat mat = shlex::read(std::cin, opts);
	shlex::write(mat, opts);
	return EXIT_SUCCESS;
}
