// gcc -o pss pss.c -lprocps

#include <stdio.h>
#include <string.h>
#include <proc/readproc.h>

int main(int argc, char** argv) {
  proc_t proc_info;
  memset(&proc_info, 0, sizeof(proc_info));

  PROCTAB* proc = openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS);
  while (readproc(proc, &proc_info) != NULL) {
    printf("%20s:\t%5ld\t%5lld\t%5lld\n",
           proc_info.cmd, proc_info.resident,
           proc_info.utime, proc_info.stime);
  }
  closeproc(proc);
}
