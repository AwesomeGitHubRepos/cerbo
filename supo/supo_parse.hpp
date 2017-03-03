#pragma once

#include <istream>
#include <vector>
#include <string>

namespace supo {
	typedef std::vector<std::string> strings;
	typedef std::vector<strings> strmat;
	strings tokenize_line(std::string &input);
	strmat tokenize_stream(std::istream& istr);
	
	std::string trim(std::string& str); // TODO should be sometwhere else

} // namespace supo
