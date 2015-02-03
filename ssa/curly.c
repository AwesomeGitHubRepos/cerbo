#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h> 
#include <pthread.h>
#include <unistd.h>

#include <curl/curl.h>

#include "curly.h"
#include "logging.h"
#include "parser.h"

// TODO put somewhere resuable 
void removeChar(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

inodes curlys;

void init_curly(curly *c, char *e, char *t)
{
  c->e = e;
  c->t = t;
  c->ok = false;
  c->magic = 0xD010AD;
}

void print_curly(curly *c)
{
  printf("Curly: Ticker=%s, last=%f, change=%f, changepc=%f, magic=%x (s/b 0xD010AD), ok=%s.\n", 
	 c->t, c->last, c->change, c->changepc, c->magic, c->ok?"true":"false");
}

void insert_curly(char *e, char *t)
{
  curly *c = (curly *)malloc(sizeof(curly));
  assert(c);
  init_curly(c, e, t);

  insert_inodes(&curlys, c);

}


#define PCEQ(targ, el) { if(strcmp(targ, p.args[0]) ==0 && p.nargs>=3) {c->el = atof(p.args[2]); nfound++;}} ;
void parse_curly(curly *c)
{
    removeChar(c->response, ',');
    c->ok = false;
    int nfound = 0;
    if(strlen(c->response) == 0) goto skip ; // will crash otherwise. Possible bug in clib??
    FILE *stream = fmemopen(c->response, strlen(c->response), "r");
    parser p;
    init_parser(&p, stream);
    while(parse(&p)) {
      //puts(p.args[0]);
      PCEQ("l_fix", last) ;
      PCEQ("c_fix", change) ;
      PCEQ("cp_fix", changepc);
    }
  skip:
    c->ok = nfound == 3;
  
    if (get_verbose()) print_curly(c);
}



void parse_curlys()
{
  curly *c;
  while(c = linode(&curlys)) parse_curly(c);
}

size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata)
{
  char *response = (char *) userdata;
  strncat(response, ptr, size * nmemb);
  return size * nmemb;
} 

static void *pull_one_url(void *td)
{
  CURL *curl;
  char url[100];
  curly *c = (curly *)td;
  sprintf(url, "http://finance.google.com/finance/info?client=ig&q=%s:%s", c->e, c->t);
  verbose("URL: <%s>\n", url);
  c->response[0] = '\0';
 
  
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, c->response);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_perform(curl); /* ignores error */ 
  curl_easy_cleanup(curl);
 
  return NULL;
}
 
void activate_curly(curly *c)
{
  //for(i=0; i< curlys.nnodes; i++) {
  //curly *c = (curly *)curlys.pnodes[i];
  assert(c->magic == 0xD010AD);
  //puts("a");
  int error = pthread_create(&(c->tid),
			 NULL, /* default attributes please */
			 pull_one_url,
			 c);
  if(0 != error)
    fprintf(stderr, "Couldn't run thread %s, errno %d\n", c->t, error);
  else
    verbose("Thread gets %s\n", c->t);
  
}

void wait_curly(curly *c)
{
 /* wait thread to terminate */
  //for(i=0; i< curlys.nnodes; i++) {
  //curly *c = (curly *)curlys.pnodes[i];
  int error = pthread_join(c->tid, NULL);
  verbose("Thread %s terminated\n", c->t);
  
}


void fetch_curlys()
{
  curl_global_init(CURL_GLOBAL_NOTHING);

  curly *c;
  /* NB the following 2 actions MUST be done separtely, not in the same loop */
  while(c = linode(&curlys)) activate_curly(c);
  while(c = linode(&curlys)) wait_curly(c); 

  curl_global_cleanup();
}

 
int main_curly(int argc, char **argv)
{

  set_verbose(true);

  /* parse command-line arguments */
  int c;
  char pathspec[] = "/home/mcarter/.ssa/fetches/%s";
  bool fetch = true;
  //bool do_down = true;
  //bool do_parse = true;
  unsigned char curly_help[] = {
    #include "curly.xxd"
    , 0x000 };
  while((c = getopt(argc, argv, "hx")) != -1)
    switch(c) {
      //case 'd': do_parse = false ; break;
    case 'h': printf("%s\nPATHSPEC=%s", curly_help, pathspec) ; return EXIT_SUCCESS;
    case 'x': fetch = false ; break;
    default: fprintf(stderr, "Ignoring unknown option '-%c'.\n", c);
    }
  

  /* common data structures */
  char *epics[] = {"AZN", "BOGUS", "ULVR", "VOD", 0} ;

  //char *epics[] = {"AZN",  "ULVR", "VOD", 0} ;
  char **pepic = epics;
  char *epic;


  /* create the curlys */
  init_inodes(&curlys);
  while(epic = *pepic) {
    insert_curly("", epic);
    pepic++;
  }

  //  puts("Finished");
  //return EXIT_SUCCESS;

  int i;
  FILE *fp;
  char full[MAXPATHLEN];

  if(fetch) {
    fetch_curlys();

    puts("Responses:");
    for(i=0; i< curlys.nnodes; i++) {
      curly *c = (curly *)curlys.pnodes[i];
      sprintf(full, pathspec, c->t);
      fp = fopen(full, "w");
      assert(fp);
      fwrite(c->response, 1, strlen(c->response), fp);
      fclose(fp);
      printf("==============\n%s\n%s\n", c->t, c->response);
    }
    puts("Done");

  } else {
    /* no fetching, so restore them from file */
     for(i=0; i< curlys.nnodes; i++) {
      curly *c = (curly *)curlys.pnodes[i];
      sprintf(full, pathspec, c->t);
      fp = fopen(full, "r");
      assert(fp);
      size_t len = fread(c->response, 1, 999, fp);
      c->response[len] = '\0';
      //printf("***  %zu ***%s***\n", len, c->response);
      fclose(fp);
     }
  }


  parse_curlys();



  /* TODO cleanup resource */

  return EXIT_SUCCESS;
}
