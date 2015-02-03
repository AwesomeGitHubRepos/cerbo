#ifndef COMMANDS_H
#define COMMANDS_H

#include "parser.h"

#define prim static void


void parse_command(parser *p);
void parse_data_file1(char *fname);
void parse_dir(char *dirname);
void parse_file1(char *full);
void parse_rc_file();


#endif //COMMANDS_H
