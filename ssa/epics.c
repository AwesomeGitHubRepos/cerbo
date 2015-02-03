#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"


void accum_comm_etran(char *folio, comm *c, double *qty, 
	double *cost)
{
	//printf("Accumulating folio %s\n", folio);

	bool all = strlen(folio) == 0;
	for(int j =0; j<etrans.nnodes; j++) {
		etran *e = (etran *)etrans.pnodes[j];

		bool fmatch = all || (strcmp(e->folio, folio) == 0);
		bool cmatch = strcmp(e->sym, c->sym) == 0;
		bool match;
/*
		if (all) {
			match = strcmp(e->folio, "ut") != 0;
		} else {
			match = fmatch;
		}

		match = match && cmatch;
*/

		match = fmatch && cmatch;
		if (match) {
			double eqty = e->qty;
			pennies ecostp = e->valuep;
			if( eqty > 0) { *cost += ecostp; }
			else { *cost *= (*qty + eqty)/ *qty; }
			*qty = *qty + eqty;
			if (fabs(*qty) < 1.0) {
				*cost = 0; // reset cost to - if we've sold everything
				*qty = 0.0;
			}
		}
	}
	//printf("Qty = %f\n", *qty);
}

void re_by(FILE *fout, char *folio)
{
	bool all = strlen(folio) == 0;

	fprintf(fout, "PORTFOLIO ");
	if (all) {
		fprintf(fout, "ALL");
	} else {
		fprintf(fout, "%s", folio);
	}

	fprintf(fout, "\n # %5s ", "SYM");
	print_money_hdrs(fout, 5, "QTY", "UCOST", "UVALUE", "COST", "VALUE");
	fprintf(fout, "%6s\n", "RET%");

	double tcost = 0.0, tvalue = 0.0;
	int count = 0;
	for(int i=0; i<comms.nnodes; i++) { 
		comm *c = (comm *)comms.pnodes[i];
		double qty = 0.0;
		double cost = 0.0;
		accum_comm_etran(folio, c, &qty, &cost); 
		assert(qty>=0.0);
		cost = cost/100.0f;
		double ucost = cost/qty;
		double uvalue = c->end_price/100.0f;
		double value = qty * uvalue;
		if(all) {
			c->end_qty = qty;
			c->ucost = ucost;
		}
		if (qty == 0.0f) continue;
		
		tcost += cost;
		tvalue += value;
		
		count++;
		fprintf(fout, "%2d %5s ", count, c->sym);
		print_monies(fout, 5, qty, 100.0 * ucost, 
			100.0 * uvalue, cost, value);
		double retpc = gainpc(value, cost);
		fprintf(fout, "%6.2f ", retpc);
		//printf("*** QTY = %f\n", qty);
		fprintf(fout, "\n");
		if (count % 3 == 0) fprintf(fout, "\n");
		//if(all) { c->end_qty = qty; }
	}

	// print footer
	fputchars(fout, 9, ' ');
	print_money_chars(fout, 3, ' ');
	print_monies(fout, 2, tcost, tvalue);
	double retpc = gainpc(tvalue, tcost);
	fprintf(fout, "%6.2f", retpc);
	fprintf(fout, "\n\n");
	fputchars(fout, 64, '-');
	fprintf(fout, "\n");
}

void print_zeros(FILE *fout)
{
	fprintf(fout, "zeros:\n");
	int count = 0;
	for(int i=0; i<comms.nnodes; i++) {
		comm *c = (comm *) comms.pnodes[i];
		if(fabs(c->end_qty) < 1 && c->type != 'I') {
			count++;
			if(count % 5 == 1) fprintf(fout, "\n");
			fprintf(fout, "%4s ", c->sym);
		}
	}
	
	fputs("\n", fout);
        fputchars(fout, 64, '-');
	fputs("\n", fout);

			
}




void report_epics() {
	FILE *fout = fout_cache("epics.txt");
	//puts("report_epics() called");
	re_by(fout, "");
	print_zeros(fout);
	re_by(fout, "hal");
	re_by(fout, "hl");
	re_by(fout, "tdi");
	re_by(fout, "tdn");
	re_by(fout, "ut");
	fclose_cache(fout);
}

