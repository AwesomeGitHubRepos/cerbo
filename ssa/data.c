#include <assert.h>
#include <errno.h>
#include <libgen.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/param.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>


//#include "curly.h"
#include "data.h"
#include "commands.h"
#include "financials.h"
#include "htm.h"
#include "portfolios.h"

inodes command_strings;
inodes comms;
inodes etrans;
inodes financials;
inodes naccs;
inodes ntrans;
inodes postings;
inodes prices;
inodes returns;





#define SEQ(s1, s2) strcmp(s1, s2) == 0 

/*              0123456789 
                YYYY-MM-DD */
char start[] = "0000-01-01";
char end[]   = "9999-12-31";
char *period_end() { return end ; };

char rc_dir[MAXPATHLEN];


/* data compare */
int dcmp(char *dstamp1, char *dstamp2){ return strncmp(dstamp1, dstamp2, 10);}
bool before(char* dstamp1, char* dstamp2) {return dcmp(dstamp1, dstamp2) < 0; }
bool within(char* dstamp) { return  dcmp(start, dstamp)>=0 &&  dcmp(dstamp, end) <=0;}

/* given dstamp return -1 if dstamp < start, 0 if dstamp in [start, end], +1 toherwise */
int dslot(char *dstamp)
{ if(dcmp(dstamp, start) < 0) return -1;
  if(dcmp(dstamp, end)   > 0) return  1;
  return 0;
}


// bankers rounding to whole number
double bround(double x) 
{
  double c ,f, det;
  c = ceil(x);
  f = floor(x);

  // eliminate strange artifacts of creating "negative 0"
  if(c == 0) c = 0.0;
  if(f == 0) f = 0.0;
  // printf("c: %f ", c);

  det = c + f - 2.0*x;
  if (det < 0) {return c;}
  if (det > 0) {return f;}

  /* banker's tie */
 if(2*ceil(c/2) == c) {return c;} else {return f;};

}

pennies topennies(double pounds)
{
	// convert pounds to pennies
	double d = pounds * 100.0f;
	d = bround(d);
	return (int) d;
}

double topounds(pennies p)
{
	return ((double)p)/100.0f; 
}

int broundi(double x)
{
	// convert a double to integer using bankers rounding
	double d = bround(x);
	return (int) d;
}

	
int pstoi(char *s)
{
	//convert a string in pounds to a integer pennies using bankers rounding
	// eg. "12.50" -> 1250
	double d = atof(s);
	d = bround(d*100.0f);
	return (int) d;
}

// round to 2 dp (subject to arithmetic precision)
double round2(double x) { return bround(x*100)/100; }

  

void star() { return ; printf("."); }

void init_data()
{
  //puts("init data");
  init_inodes(&command_strings);
  init_inodes(&comms);
  init_inodes(&etrans);
  init_inodes(&financials);
  init_inodes(&naccs);
  init_inodes(&ntrans);
  init_inodes(&postings);
  init_inodes(&prices);
  init_inodes(&returns);

}


#define UEQ(x) (strcmp(x, unit) == 0)

void insert_comm(char *sym, char *fetch, char *type, char *unit, 
		 char *exchange, char *ticker, char *name)
{
  //printf("insert_comm(): %s\n", sym);
  comm *c;
  c = (comm *)malloc(sizeof(comm));
  c->sym = sym;
  c->fetch = fetch[0] == 'W';
  c->type = type[0];
  c->unit = unit;
  c->exchange = exchange;
  c->ticker = ticker;
  c->name = name;
  
  init_curly(&(c->acurly), exchange, ticker);
  c->end_qty = 0;
  c->start_price = 0;
  c->end_price = 0;
  
  insert_inodes(&comms, c);      
  star();
}




void insert_etran(char *dstamp, char * way, char* folio, char* sym, 
		  char *qty, char *amount)
{

	//printf("ETRAN $5=%s $6=%f\n", $5, $6);
	etran* e; 
	e = (etran *)malloc(sizeof(etran));
	e->dstamp = dstamp;

	char w = way[0];
	assert( w == 'B' || w == 'S');
	int buy = (w == 'B') ? 1 : -1;
	e->buy = buy;

        sprintf(e->sort , "%-5s %10s", sym, dstamp);

	e->folio = folio;
	e->sym = sym;
	
	e->qty = buy * atof(qty);
	e->valuep = buy * pstoi(amount);


	insert_inodes(&etrans, e);
	star();
}


void insert_nacc(char *acc, char *desc)
{
  // puts("insert_nacc()");
  nacc *p_nacc;
  p_nacc = (nacc *)malloc(sizeof(nacc));
  p_nacc->acc = acc;
  p_nacc->desc = desc;
  p_nacc->balp = 0;
  insert_inodes(&naccs, p_nacc);
  star();
}

nacc *find_nacc(char *acc)
{
  int nidx, j;
  for(nidx=0; nidx<naccs.nnodes; nidx++)
    { 
      nacc *p_nacc = (nacc *)naccs.pnodes[nidx];
      if (strcmp(acc, p_nacc->acc) == 0) { return p_nacc; }
    }
  return 0; // not found
}

void insert_ntran(char *dstamp, char *dr, char *cr, char* amount, char *clear,
		  char * desc)
{
  ntran *p_ntran;
  p_ntran = (ntran *)malloc(sizeof(ntran));
  p_ntran->dstamp = dstamp;
  p_ntran->dr = dr;
  p_ntran->cr = cr;
  p_ntran->p = pstoi(amount);
  p_ntran->clear = clear;
  p_ntran->desc = desc;
  insert_inodes(&ntrans, p_ntran);
  star();
}




void insert_price(char *dstamp, char *tstamp, char *sym, double value, char *unit)
{
  price* p_price;
  p_price = (price *)malloc(sizeof(price));
  p_price->dstamp = dstamp;
  p_price->tstamp = tstamp;
  p_price->sym = sym;
  p_price->price = value;
  p_price->unit = unit;
  insert_inodes(&prices, p_price);
  star();
}

price *pricen(int i)
{
  void *v;
  v = prices.pnodes[i];
  price *p;
  p = (price *)v;
  return p;
}
int compare_price(const void *p1, const void *p2)
{
  price *pp1 = (price *)p1;
  price *pp2 = (price *)p2;
  printf("%s %s\n", pp2->dstamp, pp1->dstamp);
  return strcmp( pp2->dstamp, pp1->dstamp);
}


price *find_price(char *sym, char *dstamp)
{
  int i;
  price *found = 0;
  price *p;
  //puts("finding price");
  while(p = linode(&prices)) {
    if(strcmp(sym, p->sym) != 0) continue;
    if(dcmp(p->dstamp, dstamp)>0) continue;
    if(found == 0 || dcmp(p->dstamp, found->dstamp)>=0) found = p;
  }

  return found; // could be 0
}

double get_price_or_die(char *sym, char *dstamp)
{
	price *p = find_price(sym, dstamp);
	if(p == NULL) {
		fprintf(stderr, "ERR101: Can't find price for sym \"%s\" on/before date \"%s\". Aborting.\n", sym, dstamp);
		exit(EXIT_FAILURE);
	}
	return p->price;

}
void download_prices()
{
	int i, j;

	time_t rawtime;
	struct tm * timeinfo;
	static char dstamp[11], tstamp[9]; // we reference this outside the function
	char  fname[256];
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	strftime(dstamp, 11, "%Y-%m-%d", timeinfo);
	dstamp[10] = '\0';
	strftime(tstamp,9, "%X", timeinfo);
	tstamp[8] = '\0';
	sprintf(fname,"%s/gofi/%s.txt", rc_dir, dstamp);


	// TODO REPLICATE THE LINODE EVERYWHERE
	comm *c;
	//printf("%d\n", comms.cursor);
	while(c = linode(&comms)) { if(c->fetch) activate_curly(&(c->acurly));}
	while(c = linode(&comms)) { if(c->fetch) wait_curly(&(c->acurly));}
	while(c = linode(&comms)) { if(c->fetch) parse_curly(&(c->acurly));}

	bool ok = true;
	while(c == linode(&comms)) {
	  if(! c->fetch) continue;
	  if(c->acurly.ok) continue;
	  ok = false;
	  fprintf(stderr, "ERR: Failed to download/decode symbol <%s>\n", c->sym);
	}
	if(!ok) {fprintf(stderr, "Aborting\n"); exit(EXIT_FAILURE); }


	FILE *fp = fopen(fname,"w");
	if(fp == 0) {
          fprintf(stderr, "Couldn't open file '%s' for writing\n", fname);
          fail();
	}

	while(c = linode(&comms)) {
	  if(c->fetch) { 
	    fprintf(fp, "P\t%s\t%s\t%s\t%.4f\t%s\n", dstamp, 
		    tstamp, c->sym, c->acurly.last,  c->unit);
            insert_price(dstamp, tstamp, c->sym, c->acurly.last, c->unit);
	  }
	}

	fclose(fp);
}


void dump_prices(FILE *fp)
{
  price *p;
  while(p = linode(&prices)) {
    fprintf(fp, "Rec: price\n");
    fprintf(fp, "Sym: %s\n", p->sym);
    fprintf(fp, "Dstamp: %s\n", p->dstamp);
    fprintf(fp, "Tstamp: %s\n", p->tstamp);
    fprintf(fp, "Price: %f\n", p->price);
    fprintf(fp, "Unit: %s\n", p->unit);
    fprintf(fp, "\n");
  }
}
double find_price_def(char *sym, char *dstamp, double defval)
{
  price *p = find_price(sym, dstamp);
  if(p == 0) return defval;
  return p->price;
}



comm *find_comm(char *sym)
{
	for(int i= 0; i< comms.nnodes; i++) {
		comm *c = (comm *)(comms.pnodes[i]);
		if (strcmp(c->sym, sym) == 0) return c;
	}
	return 0;
}

void today_minus(char *num_days) 
{
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strftime(end, 11, "%F", timeinfo);

  rawtime = rawtime - atof(num_days)*60*60*24;
  timeinfo = localtime ( &rawtime );
  strftime(start, 11, "%F", timeinfo);
  
  //  printf("Start: %s, End: %s\n", start, end); 

}

void describe_period(FILE *fp)
{
  fprintf(fp, "Rec: period\nBegin: %s\nEnd: %s\n\n", start, end);
}

void set_start_period(char *from) { strncpy(start, from, 10); }
void set_end_period(char *to) { strncpy(end, to, 10); }


void set_end_period_to_today()
{
  time_t rawtime;
  struct tm * timeinfo;

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  strftime(end, 11, "%F", timeinfo);
}

void period(char *from, char *to)
{
  set_start_period(from);
  set_end_period(to);
}


int cmp_etran(const void * a, const void * b)
{
  etran *e1 = *(etran **) a;
  etran *e2 = *(etran **) b;
  return strcmp(e1->sort, e2->sort);
}




/* perform all the data derivations */
void derive_data()
{
	comm *c;
        etran *e;

	while(c = linode(&comms)) {
	  //comm *c = (comm *)comms.pnodes[i];
	  c->start_price = find_price_def(c->sym, start, 0.0f) *
	    find_price_def(c->unit, start, 0.0f);
	  c->end_price = find_price_def(c->sym, end, 0.0f) *
	    find_price_def(c->unit, end, 0.0f);
	  //c->scale = find_price_def(c->unit, end, 1.0f);
	  //printf("derive_data():comm = %s, start_price =%f, end_price=%f\n", c->sym, c->start_price, c->end_price);
	}


        // ensure each etran has a comm
        while(e = linode(&etrans)) {
          c = find_comm(e->sym);
          if(c == 0) {
            fprintf(stderr, "Error trying to insert etran. COMModity not found:%s.\n", e->sym);
            dump_etran(stderr, e);
            fprintf(stderr, "Aborting.\n");
            exit(EXIT_FAILURE);
          }
          e->comm = c;
        }


	/* etrans need to be sorted by date, or else their costings 
	can come out wrong */
        qsort(etrans.pnodes, etrans.nnodes, sizeof(void *), cmp_etran);


	while(e = linode(&etrans)) {       
	  //e = (etran *)etrans.pnodes[i];
		switch(dslot(e->dstamp)) {
		case -1: // before
			e->odstamp = start;
	  		e->flowp   = 0;
			e->start_valuep = broundi(e->qty * e->comm->start_price);
			break;
		case 0: // during
			e->odstamp = e->dstamp;
			e->flowp   = e->valuep;
			e->start_valuep = 0;
			break;

		}
     
		e->end_valuep = bround(e->comm->end_price * e->qty); 
		e->profitp = e->end_valuep - e->start_valuep - e->flowp;
	}
  
	create_postings();


}



void dump_data()
{
	FILE *fp = fout_cache("dump.txt");

	time_t mytime;
	mytime = time(NULL);
	fprintf(fp, "Rec: dump\nGenerated: %s\n\n", ctime(&mytime));

	describe_period(fp);
	dump_comms(fp);
	dump_etrans(fp);
	dump_prices(fp);
	fclose(fp);
}


comm *get_comm(int n) { return (comm *) comms.pnodes[n]; }
etran *get_etran(int n) { return (etran *)etrans.pnodes[n]; }

void print_etb()
{

	int nidx, j;
	for(nidx=0; nidx<naccs.nnodes; nidx++) { 
		nacc *p_nacc = (nacc *)naccs.pnodes[nidx];
		pennies bal = 0;
		char fname[100];
		strcpy(fname, "etb/");
		strcat(fname, p_nacc->acc);
		strcat(fname, ".txt");
		FILE *fp = fout_cache(fname);
		fprintf(fp, "%s\n", p_nacc->acc);

		for(j=0; j<postings.nnodes; j++)
		{
			posting *p_posting = (posting *)postings.pnodes[j];
			//if(strcmp(p_nacc->acc, p_posting->acc) == 0) {
			if(p_nacc == p_posting->acc) {
				bal += p_posting->amount;
				print_posting(fp, p_posting);
				print_penny(fp, bal);
				fprintf(fp, "\n");
			}
		}
		p_nacc->balp = bal;
		fclose_cache(fp);
	}

	FILE *fp = fout_cache("etb.txt");
	fputs("Extended trial balance\n", fp);
	for(nidx=0; nidx<naccs.nnodes; nidx++) { 
		nacc *p_nacc = (nacc *)naccs.pnodes[nidx];
		fprintf(fp, "%4.4s ", p_nacc->acc);
		print_penny(fp, p_nacc->balp);
		fprintf(fp, "\n");
	}
	fclose_cache(fp);
      
}



void rc_subpath(char *outdir, char *dirname)
{
	strcpy(outdir, rc_dir);
	strcat(outdir, "/");
	strcat(outdir, dirname);
}

void parse_rc_subdir(char *dirname)
{
	char full[MAXPATHLEN];
	rc_subpath(full, dirname);
	parse_dir(full);
}

void mk_rc_subdir(char *subdir)
{
	int mode = S_IRWXU | S_IRWXG ; // users and groups get full access
	char full[MAXPATHLEN];
	rc_subpath(full, subdir);

	if( access(full, F_OK) == 0) return; // already exists
	int result = mkdir(full, mode);

	if(result == 0)  return ; 

	fprintf(stderr, "mk_rc_subdir(): Can't create dir '%s'\n", full);
	fprintf(stderr, "Err number: %d\n", result);
	fail();
}
void init_dirs()
{
	strcpy(rc_dir, getenv("HOME"));
	strcat(rc_dir, "/.ssa");
	int mode = S_IRWXU | S_IRWXG ; // users and groups get full access
	mk_rc_subdir("");
	mk_rc_subdir("gofi");
	mk_rc_subdir("etb");
	//mk_rc_subdir("fetches");

	write_htm_template(rc_dir);
}

FILE *fout_cache(char *fname)
{
        char full[MAXPATHLEN];
	rc_subpath(full, fname);
	//strcat(full, "/");
        //strcat(full, fname);
	//printf("fout_cachefout_cache() open %s\n", full);
        FILE *fp = fopen(full, "w");
	if (fp == 0) {
		fprintf(stderr, 
			"ERR: fout_cache(): can't create file '%s'\n",
			full);
		fprintf(stderr, "Aborting.\n");
		exit(EXIT_FAILURE);
	}
        //assert(fp);
        return fp;
}


void print_penny(FILE *fp, int pennies)
{

  /* This method looks convoluted, but as at 04-Oct-2014, I think it is 
     necessary. There is anomolous output sometimes if you just use:
     fprintf(fp, "%'11.2f ", ((float) pennies)/100.0f);
  */

  int q = trunc((double) pennies / 100.0f);
  int r = abs(pennies - q * 100);
  fprintf(fp, "%'8d.%02d ", q, r);
}

void print_pennies(FILE *fp, int count, ...)
{
        va_list ap;
        va_start(ap, count);
        for(int i=0; i<count; i++) {
                int val = va_arg(ap, int);
		print_penny(fp, val);
        }
        va_end(ap);
}

void print_monies(FILE *fp, int count, ...)
{
        va_list ap;
        va_start(ap, count);
        for(int i=0; i<count; i++) {
                double d = va_arg(ap, double);
                fprintf(fp, "%10.2f ", d);
        }
        va_end(ap);
}


void print_money_hdrs(FILE *fp, int count, ...)
{
	va_list ap;
        va_start(ap, count);
        for(int i=0; i<count; i++) {
		char * s = va_arg(ap, char *);
		fprintf(fp, "%10.10s ", s);
        }
        va_end(ap);
}

void print_money_chars(FILE *fp, int n , char c)
{
	for(int i=0; i<n; i++) {
		for(int j=0; j<10; j++) fputc(c, fp);
		fputc(' ', fp);
	}
}

void fputchars(FILE *fp, int n, char c)
{
	for(int i=0; i<n; i++) fputc(c, fp);
}


void print_penny_field(FILE* fp, char *name, pennies p)
{
	fprintf(fp, "%s: ", name);
	print_penny(fp, p);
	fprintf(fp, "\n");
}

void dump_etran(FILE *fp, etran *e)
{
  fprintf(fp, "Rec: etran\n");
  fprintf(fp, "Dstamp: %s\n", e->dstamp);
  fprintf(fp, "Buy: %d\n",  e->buy);
  fprintf(fp, "Folio: %s\n",  e->folio);
  fprintf(fp, "Sym: %s\n",  e->sym);
  print_penny_field(fp, "Start_valuep", e->start_valuep);
  print_penny_field(fp, "Flow", e->flowp);
  print_penny_field(fp, "Profit", e->profitp);
  print_penny_field(fp, "End_valuep", e->end_valuep);
  fprintf(fp, "\n\n");
}

void dump_etrans(FILE *fp)
{
  //fputs("TBL: etrans\n", fp);
  etran *e;
  while(e = linode(&etrans)) dump_etran(fp, e);
}

void dump_comms(FILE *fp)
{
  //fputs("Comms:\n", fp);
  comm *c;
  while(c = linode(&comms)) {
    fprintf(fp, "Rec: comm\n");
    fprintf(fp, "Sym: %s\n", c->sym);
    fprintf(fp, "Fetch: %c\n", (c->fetch ? 'T' : 'F') );
    fprintf(fp, "Type: %c\n", c->type);
    fprintf(fp, "Unit: %s\n", c->unit);
    fprintf(fp, "Exchange: %s\n", c->exchange);
    fprintf(fp, "Ticker: %s\n", c->ticker);
    fprintf(fp, "Name: %s\n", c->name);
    fprintf(fp, "Start_price: %f\n", c->start_price);
    fprintf(fp, "End_price: %f\n\n", c->end_price);
  }
}

void insert_return( char *idx, char *dstamp, char *mine, char *asx)
{
	ret *r = (ret *)malloc(sizeof(ret));
	r->idx = atoi(idx);
	r->dstamp = dstamp;
	r->mine = atof(mine);
	r->asx = atof(asx);
	insert_inodes(&returns, r);
}

double gainpc(double num, double denom)
{
	return (100.0f *num)/denom -  100.0f;
}


//extern double portfolio_mine,  portfolio_ftas;
void report_returns()
{
	FILE *fp = fout_cache("returns.txt");

	char fmt1[] = "%3s %11s %6s %6s %4s %6s %6s\n";
	fprintf(fp, fmt1, "IDX", "DSTAMP", "MINE", "MINE%", 
		"ASX", "ASX%", "OUT%");

	char fmt[] = "%3d %11s %6.2f %6.2f %4.0f %6.2f %6.2f\n";
	double mine_prev, asx_prev, asx0;
	int i;
	for(i = 0; i < returns.nnodes; i++) {
		ret * r = (ret *) returns.pnodes[i];
		if( i == 0) { mine_prev = r->mine ; asx0 = r->asx ; asx_prev = r->asx ;}
		double mine_pc = gainpc(r->mine, mine_prev);
		double asx_pc  = gainpc(r->asx, asx_prev);
		double out = mine_pc - asx_pc;
		fprintf(fp, fmt, r->idx, r->dstamp, r->mine, mine_pc, 
			r->asx, asx_pc, out);
		//typedef struct ret {
        	//int idx; char *dstamp; double mine; double asx; } ret;
		mine_prev = r->mine;
		asx_prev  = r->asx;
	}

	//char fmt[] = "%3s %11s %6.2f %6.2f %4.0f %6.2f %6.2f\n";
	double mine = mine_prev * (1.0f + portfolio_mine/100.0f);
	double asx  = find_comm("FTAS")->end_price; 
	//printf("ASX=%f\n", asx);
	fprintf(fp, fmt, i, end, mine, portfolio_mine, asx,  
		portfolio_ftas, portfolio_mine - portfolio_ftas);

	double mine_pa = gainpc(pow(mine/100.0f, 1.0f / i), 1.0f);
	double asx_pa  = gainpc(pow(asx/asx0, 1.0f/i), 1.0f); 
	double out_pa  = gainpc(pow(mine * asx0 / asx/100.0f, 1.0f/i), 1.0f);
	char fmts[] = "%15s %6s %6.2f %4s %6.2f %6.2f\n";
	fprintf(fp, fmts, "AVG", " ", mine_pa, " ", asx_pa, out_pa);


	fclose_cache(fp);
}

void report_etrans()
{
	FILE *fp = fout_cache("etrans.txt");
	fprintf(fp, "%5s %10s %3s %5s %11s %11s %11s\n", "SYM", "DSTAMP", 
		"WAY", "FOLIO", "QTY", "AMOUNT", "UNIT");
	int i;
	for(i = 0; i < etrans.nnodes; i++) {
		etran *e = (etran *) etrans.pnodes[i];
		char way = e->buy == 1 ? 'B' : 'S';
		pennies amount = e->valuep;
		double qty = e->qty;
		double unit = amount / qty;
		fprintf(fp, "%5s %10s   %c %5s %11.3f %11.2f %11.4f\n",
			e->sym, e->dstamp, way, e->folio, qty,
			amount/100.0f, unit);
	}
	fclose_cache(fp);
}


void fclose_cache(FILE *fp)
{
	time_t mytime;
	mytime = time(NULL);
	fprintf(fp, "\n\nGenerated: %s\n", ctime(&mytime));
	fclose(fp);
}

void create_snapshot() {

	int i;
	int snapshot_count = 0;
	comm *c;

	puts(" #  SYM    QTY      UVALUE      VALUE      PROFIT   CHG%");

	pennies tprofit = 0;
	pennies tvalue  = 0;
	//puts("calling create_snapshot");
	while(c = linode(&comms)) {
	  //printf("%f\n", c->end_qty);
	  if( (c->end_qty != 0)  && (c->type == 'Y') ) {
	    curly *cly = &c->acurly;
	    if(!cly->ok) {
	      puts("Problem with a curly (download). Check that download is set to 'W'");
	      print_curly(cly);
	      exit(EXIT_FAILURE);
	    }

	    
	    snapshot_count++;
	    pennies value = topennies(c->end_price * c->end_qty/100.0f);
            //printf("price=%f, qty = %f, scale=%f, value=%d\n", 
            //       c->end_price, c->end_qty, c->scale, value);

	    tvalue += value;
	    double scale = get_price_or_die(c->unit, period_end());
	    pennies profit = topennies(cly->change * scale * c->end_qty/100.0f); // TODO is this rightg?
	    tprofit += profit;
	    double chgpc  = cly->changepc;
	    printf("%2d %4s %7.0f %10.4f",
		   snapshot_count, c->sym, c->end_qty, 
		   c->end_price/scale);
            print_pennies(stdout, 2, value, profit);
            printf("%6.2f\n", chgpc);
	    if (snapshot_count % 3 == 0) puts("");
	  }
	}

	printf("%26s", " ");
        print_pennies(stdout, 2, tvalue, tprofit);
	printf("%6.2f\n", 100.0f * tprofit/tvalue);

	c = find_comm("FTAS");
	assert(c);
	printf("FTAS chg:%5.2f%%\n", c->acurly.changepc);
}

void fail()
{
	fprintf(stderr, "Aborting.\n");
	free_resources();
	exit(EXIT_FAILURE);
}

void free_inodes(inodes *pi)
{
  int i;
  for(i=0; i< pi->nnodes; i++) free(pi->pnodes[i]);
  pi->nnodes = 0;
				    
}
void free_resources()
{

	free_inodes(&postings);
	free_inodes(&etrans);
	free_inodes(&financials);	
	free_inodes(&ntrans); // TODO make clear a char rather than char*	
	free_inodes(&comms);
	free_inodes(&naccs);
	free_inodes(&prices);
	free_inodes(&command_strings);
	free_inodes(&returns);


}
