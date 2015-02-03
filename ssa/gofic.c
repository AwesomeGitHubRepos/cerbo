#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "curly.h"

/*
int main_single_threaded (int argc, char **argv)
{
	int i;
	char response[2000];
	for(i = 1; i< argc; i++) {
		puts(argv[i]);
		goovals a_goovals;
		puts("Calling get_gofi");
		get_gofi(argv[i], response, 2000);
		puts(response);
		puts("Calling decode_response");
		decode_response(response, &a_goovals);
		//puts(response);
	}
	//argp_parse (&argp, argc, argv, 0, 0, 0);
	exit (0);
}
*/

int main(int argc, char **argv)
{
  puts("EPIC    PRICE    CHG+    CHG%");
  int i;
  init_inodes(&curlys);
  for(i = 1; i< argc; i++) {

    // look for exchange:ticker combo
    char *arg1 = strtok(argv[i], ":");
    char *arg2 = strtok(NULL, ":");
    //printf("e=%s, t=%s\n", arg1, arg2);
    if(arg2) { insert_curly(arg1, arg2);}
    else { insert_curly("", arg1); }

  }
  
  fetch_curlys();
  parse_curlys();

  //goovals *pgv;
  for(i=0; i<curlys.nnodes; i++) {
    curly *c = (curly *)curlys.pnodes[i];
    if(! c->ok) {
      fprintf(stderr, 
	      "ERR: Failed for ticker: %s. Exiting\n",
	      c->t);
      return EXIT_FAILURE;
    }
    
    printf("%4.4s %8.2f %7.2f %7.2f\n", c->t, c->last,
	   c->change,  c->changepc);
  }
  return EXIT_SUCCESS;
}
