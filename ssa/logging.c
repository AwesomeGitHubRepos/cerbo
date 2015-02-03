#include <stdio.h>
#include <stdarg.h>

#include "logging.h"

bool is_verbose = false; // do we want verbose mode?
bool get_verbose() { return is_verbose;}
void set_verbose(bool mode) { is_verbose = mode;};

void verbose(char *format, ...)
{
	//printf("bool=%d\n", is_verbose);
	if(! is_verbose) return;

	va_list args;
   	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

