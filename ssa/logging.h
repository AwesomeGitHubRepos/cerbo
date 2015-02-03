#ifndef LOGGING_H
#define LOGGING_H

#include <stdbool.h>

bool get_verbose();
void set_verbose(bool mode);
void verbose(char *format, ...); // print verbose

#endif //LOGGING_H
