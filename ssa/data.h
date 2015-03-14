#ifndef DATA_H
#define DATA_H

#include <sys/param.h> // for MAXPATHLEB

//#include "http.h"
#include "curly.h"
#include "inodes.h"



typedef int pennies;

/* derived items begin with d_ */

typedef struct comm {
  // fields taken from the comm command
  char *sym;
  bool fetch;
  char type; // just the first character in the input (Y, O, I)
  char *unit;
  char *ticker;
  char *exchange;
  //char *gepic; 
  char *yahoo;
  char *name;
  
  // derived fields
  curly acurly;
  //double scale;
  //goovals *goo; // obtained by downloading from google
  double end_qty; 
  double ucost;
  double start_price;
  double end_price;
} comm;


extern inodes comms;


typedef struct etran {
  char *dstamp; int buy; char *folio; char *sym; float qty;
  pennies valuep;
  

  // derived values
  char *odstamp;
  char sort[80]; // used for sorting the etrans
  pennies start_valuep, flowp, profitp, end_valuep;
  comm *comm;

} etran;
extern inodes etrans;

typedef struct financial {
	char action;
	char *param1;
	char *param2;
} financial;
extern inodes financials;

typedef struct nacc {char *acc; char *desc; pennies balp; } nacc;
extern inodes naccs;
nacc *find_nacc(char *acc);

typedef struct ntran {  
	char *dstamp; char *dr; char *cr; pennies p; char *clear;
	char * desc;} ntran;
extern inodes ntrans;



typedef struct price {
  char *dstamp, *tstamp, *sym; double price; char *unit; } price;
extern inodes prices;

typedef struct ret {
	int idx; char *dstamp; double mine; double asx; } ret;
extern inodes returns;
void insert_return( char *idx, char *dstamp, char *mine, char *asx);
void report_returns();

double bround(double x); // bankers rounding to whole number
double round2(double x); // round to 2 dp (subject to arithmetic precision)

void init_data();
void dump_data();
//void report();



void derive_data();
void insert_comm(char *sym, char *fetch, char *type, char *unit, 
		 char *exchange, char *ticker, char *yahoo, char *name);

//prim insert_comm(pline *p);

comm *get_comm(int n);
etran *get_etran(int n);
void dump_comms(FILE *fp);
comm *find_comm(char *sym);

void insert_etran(char *dstamp, char * way, char* folio, char* sym, 
		  char *qty, char *amount);
void insert_ntran(char *dstamp, char *dr, char *cr, char *amount, char *clear,
		  char * desc);
void dump_etran(FILE *fp, etran *e);
void dump_etrans(FILE *fp);
void report_etrans();

// accounts
void insert_nacc(char *acc, char *desc);
void print_etb();

// prices
void insert_price(char *dstamp, char *tstamp, char *sym, 
                  double value, char *unit);
price *find_price(char *sym, char *dstamp);
double find_price_def(char *sym, char *dstamp, double defval);
void download_prices();

// period items
extern char start[];
extern char end[];
int dcmp(char *dstamp1, char *dstamp2);
void describe_period(FILE *fp);
void period(char *from, char *to);
void today_minus(char *num_days);
void set_start_period(char *from);
void set_end_period(char *to);
void set_end_period_to_today();
char *period_end();

// postings
typedef struct posting {  
	char *dstamp; nacc *acc; nacc *alt; pennies amount; 
	char * desc;} posting;
extern inodes postings;
void create_postings();
void free_postings();
void print_posting(FILE *fp, posting *p);
void print_postingln(FILE *fp, posting *p);
void print_postings();


double topounds(int pennies);
double gainpc(double num, double denom);

// file routines
void init_dirs();
//static void recursive_mkdir(char *path, mode_t mode);
extern char rc_dir[MAXPATHLEN];

void parse_rc_subdir(char *dirname);
FILE *fout_cache(char *fname);
void fclose_cache(FILE *fp);
void print_monies(FILE *fp, int count, ...);
void print_money_hdrs(FILE *fp, int count, ...);
void print_money_chars(FILE *fp, int n , char c);
void print_penny(FILE *fp, int pennies);
void print_pennies(FILE *fp, int count, ...);
void fputchars(FILE *fp, int n, char c);
void create_snapshot();
void fail();
void free_resources();
void free_inodes(inodes *pi);


#endif /* DATA_H */
