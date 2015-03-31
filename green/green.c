// http://stackoverflow.com/questions/3219393/stdlib-and-colored-output-in-c

#include <stdio.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define BUF_SIZE 1024

int main (int argc, char const *argv[]) {

  char buffer[BUF_SIZE] ;
  //char * begin;
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

    /*
  printf(ANSI_COLOR_RED     "This text is RED!"     ANSI_COLOR_RESET "\n");
  printf(ANSI_COLOR_GREEN   "This text is GREEN!"   ANSI_COLOR_RESET "\n");
  printf(ANSI_COLOR_YELLOW  "This text is YELLOW!"  ANSI_COLOR_RESET "\n");
  printf(ANSI_COLOR_BLUE    "This text is BLUE!"    ANSI_COLOR_RESET "\n");
  printf(ANSI_COLOR_MAGENTA "This text is MAGENTA!" ANSI_COLOR_RESET "\n");
  printf(ANSI_COLOR_CYAN    "This text is CYAN!"    ANSI_COLOR_RESET "\n");
    */
  return 0;
}
