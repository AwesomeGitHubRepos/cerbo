#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

extern inodes financials;

void process_financial(FILE *fout, financial *f);

pennies stack[10];
int stackidx = 0;

void inject(FILE *fout, char *acc, int val, char *dest) {
	int i ;
	fprintf(fout, "%4.4s", acc);
	assert(strlen(dest) > 27);
	for(i= 4; i< 27; i++) { fputc(dest[i], fout); };
	fprintf(fout, "%'11.2f", topounds(val)); // 1000's separtor not standard
	for(i=38; i< strlen(dest); i++) { fputc(dest[i], fout); };
	fprintf(fout,"\n");
	fflush(fout);
}

void print_stack() {
	int i;
	printf("Stack: ");
	for(i = 0; i<= stackidx; i++) printf("%d ", stack[i]);
	puts("");
}

void insert_financial(char *cmd, char *a1, char *a2)
{
	financial *f;
	f = (financial *)malloc(sizeof(financial));
	assert(f);
	f->action = cmd[0];
	f->param1 = a1;
	f->param2 = a2;
	insert_inodes(&financials, f);
}



void create_financials()
{
	FILE *fout = fout_cache("financials.txt");
	assert(fout);

	stackidx = 0;
	stack[0] = 0 ;

	int i;
	for(i=0; i < financials.nnodes; i++) {
		financial *f = (financial *) financials.pnodes[i];
		process_financial(fout, f);
	}
	fclose_cache(fout);
}

void process_financial(FILE *fout, financial *f)
{
	nacc *n;
	int val;
	char c = f->action;
	switch (c) {
	case 'I' : // increment the stack pointer
		stackidx++;
		stack[stackidx] = 0;
		break;
	case 'M':
	case 'P':
		n = find_nacc(f->param1);
		assert(n);
		val = (c == 'P') ? n->balp : - n->balp;
		inject(fout, f->param1, val, f->param2);
		stack[stackidx] += n->balp;
		//puts(s);
		break;
	case 'Z' : // sero the stack
		stackidx = 0;
		stack[0] = 0;
		break;
	case 'R' : // reduce the stack by summing top two items
		val = stack[stackidx] + stack[stackidx-1];
		stack[stackidx] = 0;
		stackidx--;
		stack[stackidx] = val;
		break;
	case 'S' :
		fprintf(fout, "%s\n", f->param1);
		fflush(fout);
		break;
	case 'T' : // print the top of the stack as a credit
	case 'U' : // print the top of the stack as a debit
		//assert(stackidx>0);
		val = stack[stackidx]; 
		if (c == 'T') val = -val ;
		inject(fout, " ", val, f->param2);
		//stackidx++;
		//stack[stackidx] = 0.0;
		break;
	default :
		printf("FIN ERR: Unhandled case '%c'\n'", c);
	}
}
