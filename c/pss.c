// gcc -o pss pss.c -lprocps

// See discussion about possible segfault here:
// https://github.com/blippy/cerbo/issues/1#issuecomment-312567296

#include <stdio.h>
#include <string.h>
#include <proc/readproc.h>

int main(int argc, char** argv) {
	PROCTAB* proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);
	proc_t* proc_info;
	while((proc_info = readproc(proc, NULL)) != NULL) {
		printf("%20s:\t%5ld\t%5lld\t%5lld\n",
				proc_info->cmd, proc_info->resident,
				proc_info->utime, proc_info->stime);
	}
	closeproc(proc);
	return 0;
}
