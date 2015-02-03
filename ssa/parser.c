// this is very portable between projects

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "parser.h"


#define ADVANCE {p->pos++; c = p->line[p->pos]; }
#define ISWHITE (c == ' ' || c == '\t' || c == '\r')
#define ISEND  (c == '\n' || c == '\0' || c == '#')
#define STORE { p->args[p->nargs] = p->line + p->pos ; };



bool interp_line(parser *p)
{
  char c;

 eat_white:
  ADVANCE;
  if(ISEND) return false;
  if(ISWHITE) goto eat_white;

  if(c == '"') {
    ADVANCE;
    if(ISEND) return false;
    STORE;
    while(c != '\0' &&  c!= '"') ADVANCE ;
  } else {
    STORE;
    while(c != '\0' && ! ISWHITE) ADVANCE ;
  }
  p->line[p->pos] = 0;
  return true;
}
    


bool parse(parser *p)
{
  char *s;
 sniffing:
  //if(ferror(p->stream)) return false;
  s = fgets(p->line, 999, p->stream);
  strncpy(p->line_copy, p->line, 999); // for error reporting purposes
  if(s==0) return false;
  if(feof(p->stream)) return false;
  int len = strlen(p->line);
  assert(len<999);
  if(len==0) goto sniffing;
  if(p->line[len-1] == '\n') {p->line[len-1] = '\0'; len--;}
  if(len==0) goto sniffing;

  // parse a non-blank line
  p->pos = -1;
  p->nargs = 0;
  while(interp_line(p)) {
    // got a string, let's do something with it
    assert(p->nargs<10);
    //p->arg[p->nargs] = p->line + p->pos;
    p->nargs++;
  }

  if(p->nargs == 0) goto sniffing; // false alram, try again

  return true;
}

void init_parser(parser *p, FILE *stream)
{
  p->stream = stream;
  p->pos = -1;
  p->nargs = 0;
  int i;
  for(i=0; i<10; i++) p->args[i] = 0;
}

void main_parser() {
  parser p;
  init_parser(&p, stdin);
  while(parse(&p)) {
    int i;
    for(i=0; i<p.nargs; i++) printf("Arg: <%s>\n", p.args[i]);
    puts("");
  }
  
}
