#include <cstdlib>
#include <iostream>

#include "supo.h"

namespace supo {
void ssystem(const std::string& command, bool report_problem_to_stderr)
{
	int ret = system(command.c_str());
	if(ret!=0 && report_problem_to_stderr)
		std::cerr 
			<< "WARN: supo::ssystem() non-zero exit on command: <"
			<< command
			<< ">"
			<< std::endl;
}




} // namespace supo
