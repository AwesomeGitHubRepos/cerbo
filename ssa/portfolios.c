#include <assert.h>
//#include "portfolios.h"
#include <stdio.h>
#include <string.h>

#include "data.h"

//FILE *fout = NULL;



int total_vbefore = 0;
int total_vflow = 0;
int total_vprofit = 0;
int total_vto = 0;

double portfolio_mine,  portfolio_ftas;

void lout(FILE *fp, char *acc, int v1, int v2, int v3, int v4)
{
	double ret = 0.0;	
	if ( v1 != 0.0) ret = 100.0 * v3/ v1;
	fprintf(fp, "%5.5s ", acc);
	// %'10.2f %'10.2f %'10.2f %'10.2f %5.1f\n", 
	print_pennies(fp, 4, v1, v2, v3, v4);
	fprintf(fp, "%6.2f\n", ret);
	if (strcmp(acc, "mine") == 0) portfolio_mine = ret;
	if (strcmp(acc, "FTAS") == 0) portfolio_ftas = ret;

}

void folio(FILE *fout, char *name) {

	int vbefore = 0;
	int vflow = 0;
	int vprofit = 0;
	int vto = 0;

	int i;
	for(i =0; i< etrans.nnodes; i++) {
		etran *e =  (etran *)etrans.pnodes[i];
		if (strcmp(e->folio, name) == 0) {
			vbefore += e->start_valuep;
			vflow += e->flowp;
			vprofit += e->profitp;
			vto += e->end_valuep;
		}
	}


	lout(fout, name, vbefore, vflow, vprofit, vto);
	total_vbefore += vbefore;
	total_vflow += vflow;
	total_vprofit += vprofit;
	total_vto += vto;
}


void display_comm_return(FILE *fout, char *sym)
{
	comm *c = find_comm(sym);
	int b4 = c->start_price;
	int ep = c->end_price;
	lout(fout, c->sym, b4, 0, ep - b4, ep); 
}

void create_portfolios() {

	FILE *fout = fout_cache("portfolio.txt");

	char *fmt = "%5.5s %11s %11s %11s %11s %6.6s\n";
	char *su = "-----------";
	char *du = "===========";

	fprintf(fout, fmt, "FOLIO", "VBEFORE", "VFLOW", "VPROFIT", 
		"VTO", "VRET");

	folio(fout, "hal");
	folio(fout, "hl");
	folio(fout, "ib");
	fputs("\n", fout); // blank line to increase readability
	folio(fout, "tdn");
	folio(fout, "tdi");
	//fout(fout, 
	fprintf(fout, fmt, su, su, su, su, su, su);
	lout(fout, "mine", total_vbefore, total_vflow, total_vprofit, 
		total_vto);
	//portfolio_mine_gain_pc = pcgain(
	//folio(fout, "mine") ;
	folio(fout, "ut");
	fprintf(fout, fmt, su, su, su, su, su, su);
	lout(fout, "all", total_vbefore, total_vflow, total_vprofit, total_vto);
	fprintf(fout, fmt, du, du, du, du, du, du);

	display_comm_return(fout, "FTAS");
	display_comm_return(fout, "FTSE");
        display_comm_return(fout, "MCX");

	fclose_cache(fout);
	
}

