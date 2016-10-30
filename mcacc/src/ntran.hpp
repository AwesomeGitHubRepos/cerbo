#pragma once

#include <string>
#include <vector>

//using namespace std {

typedef struct ntran_t {
	std::string dstamp;
	std::string dr;
	std::string cr;
	currency amount;
	std::string desc;
} ntran_t;

typedef std::vector<ntran_t> ntran_ts;

//}

