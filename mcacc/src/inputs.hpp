#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "comm.hpp"
#include "etran.hpp"
#include "nacc.hpp"
#include "ntran.hpp"
//#include "stend.hpp"
#include "yahoo.hpp"

typedef struct inputs_t {
	comm_ts comms;
	etran_ts etrans;
	nacc_ts naccs;
	ntran_ts ntrans;
	period p;
	//stend_ts stends;
	yahoo_ts yahoos;
} inputs_t;

inputs_t read_inputs();
void insert_yahoo(const yahoo_t& y, inputs_t& inputs);

namespace parse {
	std::vector<std::string> tokenise_line(std::string& s);
	vecvec_t vecvec(std::istream& istr);
	vecvec_t vecvec(const std::string& filename);
}

