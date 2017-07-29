#include <iostream>
#include <cstdlib>

#include <supo.h>

#include "common.h"
#include "show.h"

using namespace std;

void show(const std::string& report_name)
{
	string filename = s3(report_name + ".rep");
	string cmd = "less " + filename;
	supo::ssystem(cmd.c_str(), true);
}
