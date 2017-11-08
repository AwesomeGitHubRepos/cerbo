#include <iostream>
#include <map>
#include <regex>

int
main() 
{
	std::regex rgx("\\s+");

	std::map<std::string, double> m;

	std::string line;

	std::sregex_token_iterator rend;

	while(std::getline(std::cin, line)) {
		std::sregex_token_iterator it(line.begin(), line.end(), rgx, -1);		
		//std::cout << (it.end() - it.begin()) << "\n";
		if(it==rend) continue;
		std::string key = *it++;
		if(it==rend) continue;
		double value = stod(*it);

		auto mit = m.find(key);
		if(mit == m.end())
			m[key] = value;
		else
			m[key] += value;
	}

	for(auto mit=m.begin(); mit != m.end(); ++mit)
		std::cout << mit->first << "\t" << mit->second << "\n";

	return 0;
}
