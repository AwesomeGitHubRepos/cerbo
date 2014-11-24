#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <curl/curl.h>
 

struct HttpData {
  pthread_t tid;
  char *gepic;
  char response[2000];
};





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
  struct HttpData *h = (struct HttpData *)td;
  sprintf(url, "http://finance.google.com/finance/info?client=ig&q=LON:%s", h->gepic);
  printf("URL: <%s>\n", url);
  h->response[0] = '\0';
 
  
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, h->response);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
  curl_easy_perform(curl); /* ignores error */ 
  curl_easy_cleanup(curl);
 
  return NULL;
}
 
 
 
int main(int argc, char **argv)
{
  struct HttpData https[100];
  char *epics[] = {"AZN", "BOGUS", "ULVR", "VOD", 0} ;

  int i;
  int error;

  char *epic;
  epic = *epics;
  int num = 0;
  while(epics[num]) {
    https[num].gepic = epics[num];
    num++;
  }

  for(i=0; i<num; i++) { printf("Epic=<%s>\n", https[i].gepic);}
      
 
  /* Must initialize libcurl before any threads are started 
     Using CURL_GLOBAL_ALL creates a small memory leak in libcurl4.
     The problem seems to be in the SSL side.
  */
  curl_global_init(CURL_GLOBAL_NOTHING);
 
  for(i=0; i< num; i++) {
    //error = pthread_create(&tid[i],
    error = pthread_create(&(https[i].tid),
                           NULL, /* default attributes please */ 
                           pull_one_url,
                           &https[i]);
    if(0 != error)
      fprintf(stderr, "Couldn't run thread number %d, errno %d\n", i, error);
    else
      fprintf(stderr, "Thread %d, gets %s\n", i, https[i].gepic);
  }
 
  /* now wait for all threads to terminate */ 
  for(i=0; i< num; i++) {
    error = pthread_join(https[i].tid, NULL);
    fprintf(stderr, "Thread %d terminated\n", i);
  }

  curl_global_cleanup();

  puts("Results...");
  for(i=0; i<num; i++) {
    printf("%s\n%s\n\n", https[i].gepic, https[i].response);
  }

  return 0;
}
