#pragma once

#include <map>
#include <string>

typedef struct comm_t {
	std::string ticker;
	std::string down;
	std::string typ;
	std::string unit;
	std::string desc;
} comm_t;


typedef std::map<std::string, comm_t> comm_ts;
