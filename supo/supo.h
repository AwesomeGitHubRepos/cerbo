#pragma once

// an "all-inclusive" header file

#include <supo_general.h>
#include <supo_parse.h>
#include <supo_stats.h>

namespace supo {

void ssystem(const std::string& command, bool report_problem_to_stderr);

}
