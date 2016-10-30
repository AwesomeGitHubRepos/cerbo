#include <iostream>
#include <cstdlib>

#include "common.hpp"
#include "show.hpp"

using namespace std;
//void show(std::string report_name)
void show(const std::string& report_name)
{
	string filename = s3(report_name + ".rep");
	string cmd = "less " + filename;
	system(cmd.c_str());
}
