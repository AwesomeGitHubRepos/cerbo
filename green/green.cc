// http://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c

#include <getopt.h>
#include <iostream>
#include <stdio.h>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define BUF_SIZE 1024

const string help_text = R"hlp(
Options:
  -h [ --help ]    this help
  -v [ --version ] version
)hlp";

int main (int argc, char const *argv[]) {

        //options opts;
        int c;
        while(1) {
                int this_option_optind = optind ? optind : 1;
                int option_index = 0;
                static struct option long_options[] = {
                        {"help", no_argument, 0, 'h'},
                        {"version", no_argument, 0, 'v'},
                        {0 ,0,0,0}
                };
                c = getopt_long(argc, (char* const*)argv, "hv",
                                long_options,  &option_index);
                if(c == -1) break;

                switch(c) {
                        case 'h': cout << help_text; exit(EXIT_SUCCESS);
                        case 'v': cout << "green (" << PACKAGE_NAME << ") " << VERSION << "\n"; exit(EXIT_FAILURE);
                        default:  cerr << "getopt returned character code " << c << endl; exit(EXIT_FAILURE);
                }
        }
        if (optind < argc) {
                printf("non-option ARGV-elements: ");
                while (optind < argc)
                        printf("%s ", argv[optind++]);
                printf("\n");
        }



	char buffer[BUF_SIZE] ;
	int i = 0;
	size_t nread;

	printf(ANSI_COLOR_GREEN);
	while(nread = fread(buffer, 1, BUF_SIZE, stdin)){
		for(int j =0; j< nread; j++) {
			int c = buffer[j];
			if(c == '\n') {
				if(i==0) {printf(ANSI_COLOR_RESET);}
				i = (1+i) % 3;
				putchar(c);
				if(i==0) {printf(ANSI_COLOR_GREEN);}
			} else { putchar(c);}
		}
	}
	printf(ANSI_COLOR_RESET);

	return 0;
}
