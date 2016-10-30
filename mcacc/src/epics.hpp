#pragma once

#include <fstream>
#include <set>
#include <string>
#include <vector>

#include "etran.hpp"
#include "stend.hpp"

class folio_c {
	public:
		folio_c(const std::string& name): m_name(name) {};
		std::string m_name;
		detran_cs filter(const detran_cs& es) const;
		void calculate(const detran_cs& all_etrans);
		std::set<std::string> zeros;
		currency cost;
		currency flow;
		currency value;
		currency pdp;
		currency vbefore;
		currency pbd;
		void print_to_epic_file(std::ofstream& ofs) const;
		void print_to_portfolio_file(std::ofstream& ofs) const;
		std::vector<detran_c> reduced_epics;
};

typedef std::vector<folio_c> folio_cs;


folio_cs epics_main(const detran_cs& es, const stend_ts& stends);
