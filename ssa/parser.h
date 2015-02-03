#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include <stdio.h>

typedef struct {
  int pos;
  int nargs;
  char *args[10];
  char line[1000];
  char line_copy[1000];
  FILE *stream;
} parser ;

void init_parser(parser *p, FILE *stream);
bool parse(parser *p);

#endif // PARSER_H
