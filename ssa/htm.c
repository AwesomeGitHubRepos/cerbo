#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h> // for MAXPATHLEN



//extern unsigned char htm_template_text[];

unsigned char htm_template_text[] = {
#include "htm.xxd"
, 0x000 };



void write_htm_template(char *dirname)
{
	FILE *fp;
	char full[MAXPATHLEN];
	sprintf(full, "%s/ssa.htm", dirname);
	fp = fopen(full, "w");
	assert(fp);
	fprintf(fp, "%s", htm_template_text);
	fclose(fp);
}
