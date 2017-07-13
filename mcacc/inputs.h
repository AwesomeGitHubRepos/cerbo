#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "comm.h"
#include "etran.h"
#include "nacc.h"
#include "ntran.h"
#include "yahoo.h"

typedef struct inputs_t {
	comm_ts comms;
	etran_cs etrans;
	nacc_ts naccs;
	ntran_ts ntrans;
	period p;
	yahoo_ts yahoos;
} inputs_t;

inputs_t read_inputs();
void insert_yahoo(const yahoo_t& y, inputs_t& inputs);

namespace parse {
	std::vector<std::string> tokenise_line(std::string& s);
	vecvec_t vecvec(std::istream& istr);
	vecvec_t vecvec(const std::string& filename);
}

