#pragma once

#include <map>
#include <string>

//void parse_args();
typedef std::map<std::string, std::string> vm_t;

vm_t parse_args(int argc, char *argv[]);
