#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <gsl/gsl_sort.h>
//#include <gsl/gsl_statistics.h>
//#include <readline/readline.h>

#ifdef USETGETLINE
//#include <libtecla.h>
#endif

extern int yylex (void);
#include "mcstats.h"
#include "stats.h"

#define BUFLEN 4096

/* NB I have used yytext directly rather than allocated string
   space. This is fine so long as no look-ahead is required, but will
   cause problems otheriwse.
 */
extern char *yytext; 
YYSTYPE yylval;

//extern unsigned char help_text[];


//void sort_doubles(double 
#define MAXELS 7500
int nels = 0;
double yarr[MAXELS]; // data in original order
double yarrs[MAXELS]; // sorted data

// #define STR_EQ(s1,s2) (strcmp(s1,s2) == 0)
// #define CMD_EQ(s1) (STR_EQ(yytext, s1))


int cmp(const void *a, const void *b)
{
	double v1 = *(double *)a;
	double v2 = *(double *)b;
	double v3 = v1 - v2;
	return ( 0 < v3) - (v3 < 0) ;
}


void go()
{
  int i;
  double m, min, max;

  memcpy(yarrs, yarr, nels *sizeof(double));

  qsort(yarrs, nels, sizeof(double), cmp);
  //gsl_sort(yarrs, 1, nels);

  puts("sorted array:");
  for(i =0; i<nels; i++)
    { 
      m = (double)100*i/nels;
      printf("%d %lf %lf\n", i, m, yarrs[i]); 
    }
  
  //gsl_stats_minmax(&min, &max, yarr,1 , nels);
  min = yarrs[0];
  pstat("min", min);
  max = yarrs[nels-1];
  pstat("max", max);
  m = yarr[nels-1];
  pstat("r2r", (max -m)/(m-min)); // reward-to-risk ratio
  
  // geometric mean
  m = 1;
  for(i =0; i<nels; i++) { m*= yarr[i]; }
  m = pow(m, 1.0/nels);
  pstat("geom", m);

  // harmonic mean
  m = 0;
  for(i =0; i<nels; i++) { m+= 1.0/yarr[i]; }
  m = nels / m;
  pstat("harm", m);
  

  m = 0.0f;
  for(i=0; i<nels; i++) { m += yarr[i]; }
  m = m/(float)nels;
  //m = gsl_stats_mean(yarrs, 1, nels);
  pstat("mean", m);
  m = (yarrs[nels/2] + yarrs[(nels-1)/2])/2.0f;
  //m = gsl_stats_median_from_sorted_data(yarrs,1, nels);
  pstat("median", m);
  
  pstat("num", (float)nels);

 	pstat("sstddev", sstdev(yarr, nels));

  m = 0.0;
  for(i=0; i<nels; i++) { m+= yarr[i] ; }
  pstat("sum", m);
  
  //m = gsl_stats_sd(yarr, 1, nels);
  //pstat("stdevs", m);
  
  exp_fit_y(yarr, nels);
  

	for(i=0; i<20; i++) { fputc('-', stdout); }
	printf("\n");

	nels = 0;
}

 


// http://stackoverflow.com/questions/5457608/c-remove-character-from-string
void remove_char(char *str, char garbage) {

    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}


void process_token(char *token)
{
  //printf("token: *%s*\n", token);
  switch(token[0])
    {
    case 'g': go(); break;
    //case 'h':  puts((const char *)help_text); break;
    case 'q': exit(0); break;
    default:
      remove_char(token, ',');
      char *pend;
      float f;
      f = strtof(token, &pend);
      if(pend[0] == '\0') // if there's junk at the end then it wont be \0
        {
                assert(nels< MAXELS);
                yarr[nels] = f;
                nels++;
        }
      else
        { puts("Rejected"); }
    }
}

void process_line(char *line)
{
  char buf[BUFLEN];
  char delim[] = " \t\r\n";
  char *token;
  strcpy(buf, line);
  token = strtok(buf, delim);
  while(token != NULL)
    {
      process_token(token);
      token = strtok(NULL, delim);
    }
}

/*
void loop_using_buffer()
{

  
 loop:
  fgets(buf, BUFLEN, stdin);
  token = strtok(buf, delim);
  while(token != NULL)
    {
      process_token(token);
      token = strtok(NULL, delim);
    }
  goto loop;
 /
}
*/

void loop_using_lex()
{
	int t;
loop:
	t = yylex();
	if (t != NEWLINE) process_token(yytext);
goto loop;
	
	
}
#ifdef USEGETLINE
/* void loop_using_getline()
{

  char *line;
  GetLine *gl;
  gl = new_GetLine(BUFLEN, 2048);
  assert(gl);
  while ((line=gl_get_line(gl, "", NULL, -1)) != NULL) {process_line(line);}
  gl = del_GetLine(gl);

}
*/
#endif

int main()
{
	puts("stats 1.0");
	puts("Common commands: gq. h for help");
	//loop_using_getline();
	//loop_using_buffer();
	loop_using_lex();
	return EXIT_SUCCESS;
}
