#pragma once

#include <map>
#include <string>
#include <vector>

#include "dec.h"

typedef struct nacc_t {
	std::string acc;
	std::string alt;
	std::string typ;
	double scale;
	std::string desc;
	currency bal;
} nacc_t;

typedef std::map<std::string, nacc_t> nacc_ts;
