#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

char* string(char *src)
{
  char *dest = malloc(strlen(src) +1);
  assert(dest);
  strcpy(dest, src);
  //insert_inodes(&command_strings, dest);
  return dest;
}
