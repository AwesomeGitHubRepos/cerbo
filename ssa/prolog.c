#include <assert.h>
#include <stdio.h>

#include "data.h"
#include "prolog.h"


//#define PRO_OUT
void export_prolog()
{
  FILE *fp = fout_cache("ssa.dl");
  assert(fp);


  etran *e;
  //puts("X");
  while(e = linode(&etrans)){
    //puts(".");
    fprintf(fp, "etran('%s', '%s', %.3f, %d)\n", e->dstamp, e->sym, e->qty, e->valuep);
  }

  fclose(fp);
}
