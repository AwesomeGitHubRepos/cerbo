#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

extern inodes postings;

void print_posting(FILE *fp, posting *p)
{
	fprintf(fp, "%s %4.4s %4.4s %30.30s ", 
	     p->dstamp, p->acc->acc, p->alt->acc, p->desc);
	print_penny(fp, p->amount);
}

void print_postingln(FILE *fp, posting *p)
{
	print_posting(fp, p); puts("");
}

int compare_posting(const void * a, const void * b)
{
  posting *p1 = *(posting **) a;
  //print_posting(p1);
  char *d1 = p1->dstamp;
  //printf("d1=%s ", d1);

  posting *p2 = *(posting **) b;
  char *d2 = p2->dstamp;
  //printf("d2=%s\n", d2);

  return dcmp(d1, d2);
}

void create_posting(char *dstamp, char *acc, char *alt, int amount, 
		    char *desc)
{
  
  if(amount == 0.00) { return ; }
  posting *p_posting;
  p_posting = (posting *)malloc(sizeof(posting));
  p_posting->dstamp = dstamp;

  nacc *n;
  p_posting->acc = 0; 
  p_posting->alt = 0;
  while(n = linode(&naccs)){
    if(strcmp(acc, n->acc)==0) p_posting->acc = n;
    if(strcmp(alt, n->acc)==0) p_posting->alt = n;
  }
  if(p_posting->acc == 0 || p_posting->alt == 0){
    printf("ERR: Can't create posting. Failed to recognise account code %s and/or %s.\n", acc, alt);
    exit(EXIT_FAILURE);
  }

 
  p_posting->amount = amount; 
  p_posting->desc = desc;
  insert_inodes(&postings, p_posting);
}

void create_postings()
{

	int i;
	//free_postings();
	free_inodes(&postings);

	// create ntran postings
	for(i=0; i<ntrans.nnodes; i++) {
		ntran *p_ntran = (ntran *)ntrans.pnodes[i];
		create_posting(p_ntran->dstamp, p_ntran->dr, p_ntran->cr,
			p_ntran->p, p_ntran->desc);
		create_posting(p_ntran->dstamp, p_ntran->cr,
			p_ntran->dr, -p_ntran->p, p_ntran->desc);
	}

	// create etran postings
	for(i=0; i<etrans.nnodes; i++) {
		etran *p_etran = (etran *)etrans.pnodes[i];
		char *odstamp = p_etran->odstamp;
		char *sym = p_etran->sym;
		/*
		create_posting(odstamp, "opn", "?", -p_etran->start_valuep, 
			sym);
		create_posting(odstamp, p_etran->folio, "?", -p_etran->flowp, sym);
		create_posting(odstamp, "pga", "?", -p_etran->profitp, sym);
		create_posting(odstamp, "prt", "?", p_etran->end_valuep, sym);
		*/

		create_posting(odstamp, "opn", "pga", -p_etran->start_valuep, 
			sym);
		create_posting(odstamp, p_etran->folio, "pga", -p_etran->flowp, sym);
		create_posting(odstamp, "pga", "pga", -p_etran->profitp, sym);
		create_posting(odstamp, "prt", "pga", p_etran->end_valuep, sym);

	}

	//print_postings();
	qsort(postings.pnodes, postings.nnodes, sizeof(void *), compare_posting);
	//print_postings();
     
}


void print_postings()
{
	FILE *fp = fout_cache("postings.txt");
	fputs("POSTINGS\n", fp);
	int i;
	for(i=0; i<postings.nnodes; i++) {
		posting *p_posting = (posting *)postings.pnodes[i];
		print_postingln(fp, p_posting);
	}
	fclose(fp);
}

