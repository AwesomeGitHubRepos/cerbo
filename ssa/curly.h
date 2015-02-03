#ifndef CURLY_H
#define CURLY_H

#include <stdbool.h>

#include "inodes.h"

typedef struct {
  pthread_t tid;
  char *e; // exchange. e.g. LON, , or just empty
  char *t; // ticker, e.g. AZN, MCX
  char response[2000];

  int magic; /* checks for initialisation */
  bool ok;
  double last;
  double change;
  double changepc;
} curly;

extern inodes curlys;

void activate_curly(curly *c);
void print_curly(curly *c);
void wait_curly(curly *c);
void parse_curly(curly *c);
void init_curly(curly *c , char *e, char *t);
void insert_curly(char *e, char *t);
void fetch_curlys();
void parse_curlys();

#endif // CURLY_H
