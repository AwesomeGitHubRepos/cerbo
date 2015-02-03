#include <assert.h>
#include <stdio.h>

#include "csv.h"

#include "data.h"

FILE *csv_fout;

void csv_str(char *str)
{
  fprintf(csv_fout, "\"%s\",", str);
}

void csv_strln(char *str)
{
  fprintf(csv_fout, "\"%s\"\n", str);
}



void create_csvs()
{

  //FILE *fout;

  csv_fout = fout_cache("comms.csv");
  assert(csv_fout);
  fprintf(csv_fout, "sym,web,type,unit,exchange,ticker,name\n");
  comm *c;
  while(c = linode(&comms)) {
    char w = c->fetch ? 'W' : '-' ;
    csv_str(c->sym);
    fprintf(csv_fout, "\"%c\",\"%c\",", w, c->type);
    csv_str(c->unit);
    csv_str(c->exchange);
    csv_str(c->ticker);
    csv_strln(c->name);
  }
  fclose(csv_fout);

  csv_fout = fout_cache("prices.csv");
  assert(csv_fout);
  fprintf(csv_fout, "dstamp,tstamp,sym,price,unit\n");
  price *p;
  while(p = linode(&prices)) {
    csv_str(p->dstamp);
    csv_str(p->tstamp);
    csv_str(p->sym);
    fprintf(csv_fout, "%.4f,", p->price);
    csv_strln(p->unit);
  }
  fclose(csv_fout);

}
