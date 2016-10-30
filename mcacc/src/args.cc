#include <getopt.h>
#include <iostream>
#include <map>
#include <string>

#include "args.hpp"
#include "common.hpp"
#include "inputs.hpp"
#include "tests.hpp"

using std::cerr;
using std::cout;
using std::endl;
using std::map;
using std::string;
using namespace std::literals;


template<class K, class V>
void reinsert(map<K, V>& m, const K& key, const V& value)
{
	m[key] = value;
}


const string help_text=R"hlp(
Options:
  --clean               Clean up the working folders
  --end YYYY-MM-DD      End date
  -h [ --help ]         Help
  --pre arg             Preprocess command
  -r [ --root ]         Print root directory
  --show arg            Show a report
  -s [ --snap ]         Snapshot
  --start YYYY-MM-DD    Start date
  -t [ --tests ]        Run tests only
  -v [ --version ]      Version
  --wiegley arg (=off)  Produce (on) or surpress (off) wiegley output
)hlp";

void exitok() { exit(EXIT_SUCCESS); }

vm_t parse_args(int argc, char *argv[])
{
	vm_t vm;
	vm["snap"] = "off";
	vm["wiegley"] = "off";

	vecvec_t ini_file = parse::vecvec(rootdir() + "/mcacc2.ini");
	for(const auto& r: ini_file) {
		if(r.size() !=2) continue;
		vm[r[0]] = r[1];
		//cout << r[0] << "=" << r[1] << endl;
	}

        //options opts;
        int c;
        while(1) {
                int this_option_optind = optind ? optind : 1;
                int option_index = 0;
                static struct option long_options[] = {
			{"clean",   no_argument, 0, 0},
			{"end",   required_argument, 0, 0},
                        {"help",    no_argument, 0, 'h'},
			{"root",    no_argument, 0, 'r'},
                        {"snap",    no_argument, 0, 's'},
			{"start",   required_argument, 0, 0},
			{"tests",   no_argument, 0, 't'},
                        {"version", no_argument, 0, 'v'},
                        {0 ,0,0,0}
                };
                c = getopt_long(argc, (char* const*)argv, "hrstv",
                                long_options,  &option_index);
                if(c == -1) break;

                switch(c) {
			case  0 : vm[long_options[option_index].name] = optarg;
				  break;
			case 'h': cout << help_text; exitok();
			case 'r': cout << rootdir() << endl;
				  exitok();
			case 's': vm["snap"] = "on"; break;
			case 't': run_all_tests(); exitok();
			case 'v': 
				  std::cout << "mcacc (" << PACKAGE_NAME;
				  cout  << ") " << VERSION << "\n";
				  exitok();
			default:  
				  cerr << "getopt returned character code ";
				  cerr << c << endl; 
				  exit(EXIT_FAILURE);
                }
        }
        if (optind < argc) {
                printf("non-option ARGV-elements: ");
                while (optind < argc)
                        printf("%s ", argv[optind++]);
                printf("\n");
        }

	//reinsert(vm, "a"s, "1"s);
	//reinsert(vm, "a"s, "2"s);
	//cout << vm["a"s] << endl;
	return vm;
}

