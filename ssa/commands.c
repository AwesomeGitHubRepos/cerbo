#include <assert.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> // for directory operations

#include "commands.h"
#include "data.h"
#include "financials.h"
#include "logging.h"
#include "utils.h"

extern inodes command_strings;

char* astring(char *src)
{
  char *dest = string(src);
  insert_inodes(&command_strings, dest);
  return dest;
}







typedef void (*codeptr)(char **args);


struct primfcn {
  char *name;
  codeptr pcode;
  int nargs;
};


void parse_data_file1(char *fname)
{
  char full[MAXPATHLEN];
  sprintf(full, "%s/redact/docs/accts2014/%s", getenv("HOME"), fname);
  //parse_file(full);
  parse_file1(full);
 
}

void parse_dir(char *dirname)
{
  DIR           *d;
  struct dirent *dir;

  char full[256];
  d = opendir(dirname);
  if (d)
    {
      while ((dir = readdir(d)) != NULL)
	{
	  if( strcmp(dir->d_name, ".") != 0 && 
	      strcmp(dir->d_name, "..") != 0)
	    {
	      full[0] = 0;
	      strcat(full, dirname);
	      strcat(full, "/");
	      strcat(full, dir->d_name);
	      //puts(full);
	      parse_file1(full);
	  }
	}
      
      closedir(d);
    }
}


void parse_file1(char *full)
{
  FILE *fp = fopen(full, "r");
  if(fp == 0) {
    fprintf(stderr, "Couldn't open file <%s>\n", full);
    abort();
  }

  //pline p;
  //p.debug = get_verbose();
  parser p;
  init_parser(&p, fp);
  while(parse(&p)) parse_command(&p) ;
  //parse_stream(fp, &p, parse_command);
  fclose(fp);
  verbose("Finished processing file %s\n", full);
}


prim P_noop(char **args) { /* do nothing */ }

prim P_insert_comm(char **args)
{
  insert_comm(astring(args[1]), args[2], args[3], astring(args[4]), 
	      astring(args[5]), astring(args[6]), astring(args[7]),
	      astring(args[8])); 
}

prim P_insert_etran(char **args)
{
  insert_etran(astring(args[1]), args[2], astring(args[3]), 
               astring(args[4]), args[5], args[6]); 
}

prim P_insert_nacc(char **args)
{
  insert_nacc(astring(args[1]), astring(args[2]));
}

prim P_insert_ntran(char **args)
{
  insert_ntran(astring(args[1]), astring(args[2]), astring(args[3]), args[4], 
	       astring(args[5]), astring(args[6]));
}

prim P_parsefile(char **args)
{
  parse_file1(args[1]);
}

prim P_insert_price(char **args) 
{
  insert_price(astring(args[1]), astring(args[2]), 
               astring(args[3]), atof(args[4]), astring(args[5]));
}

prim P_return(char **args) {   
  insert_return(args[1], astring(args[2]), args[3], args[4]); }

prim P_period(char **args)
{
  period(args[1], args[2]);
}

prim P_insert_financial(char **args)
{
  insert_financial(args[1], astring(args[2]), astring(args[3]));
}

prim P_echo(char **args)
{
  puts(args[1]);
}



prim P_parsedir(char **args)
{
  parse_dir(args[1]);
}
/*
prim P_download_method(char **args)
{
  char *method = args[1];
  switch(method[0]) {
  case 's' : dload_method = DLOAD_USING_SHELL ; break;
  case 't' : dload_method = DLOAD_USING_THREADS ; break;
  default:
    fprintf(stderr, "ERROR: Skipping Unknown download_method() <%s>.\n", method);
  }
}
*/

static struct primfcn cmd[] = {
  {"comm", P_insert_comm, 8},
  //  {"download_method", P_download_method, 1},
  {"desc", P_noop, -1},
  {"echo", P_echo, 1},
  {"etran", P_insert_etran, 6},
  {"fin", P_insert_financial, 3},
  {"fx", P_noop, -1},
  {"nacc", P_insert_nacc, 2},
  {"ntran", P_insert_ntran, 6},
  {"P", P_insert_price, 5},
  {"parsedir", P_parsedir, 1},
  {"parsefile", P_parsefile, 1},
  {"period", P_period, 2},
  {"return", P_return, 4},
  {"", NULL, 0}
};


void parse_command(parser *p)
{

  verbose("Entering parse_command()\n");
  struct primfcn *c = cmd;
  while(c->pcode) {
    if(strcmp(c->name, p->args[0]) ==0) {
      if(c->nargs >=0 && p->nargs != c->nargs +1) {
	fprintf(stderr, "Wrong number of arguments in:\n%s\nExiting.", p->line_copy);
	exit(EXIT_FAILURE);
      }
      verbose("Found command <%s>\n", c->name);
      c->pcode(p->args);
      return;
    }
    c++;
  }
  printf("ERR: Command not found: <%s>\n", p->args[0]);
}


void parse_rc_file()
{
  char rc_file[MAXPATHLEN];
  sprintf(rc_file,"%s/.ssarc", getenv("HOME"));
  parse_file1(rc_file);
}
