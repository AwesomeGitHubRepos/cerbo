#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "simclist.h"
#include "utils.h"

typedef struct { char *key; char *value; } global ;

static list_t globals;

void init_globals()
{
  list_init(&globals);
}

void set_global(char *key, char *value)
{
  // TODO handle case where key does(not) exist
  global *g = malloc(sizeof(global));
  assert(g);
  g->key = string(key);
  g->value =string(value);
  list_append(&globals, g);
}


int main_globals()
{

  init_globals();

  set_global("pipe", "cat");
  /*
  global g;
  g.key = string("pipe");
  g.value = string("cat");
  list_append(&globals, &g);
  */
  global *g1 = (global *)list_get_at(& globals, 0);
  puts(g1->key);
  puts(g1->value);
  free(g1->key);
  free(g1->value);
  free(g1);

  list_destroy(&globals);

  return EXIT_SUCCESS;
}
