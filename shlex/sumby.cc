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
		//double value = stod(*it);
		std::string vstr = *it;
		double value = atof(vstr.c_str());

		auto mit = m.find(key);
		if(mit == m.end())
			m[key] = value;
		else
			m[key] += value;

		//std::cout << "sumby:" << key << "\t" << value << "\t" << m[key] << "\n";
		//printf("sumby:%s\t%.3f\t%.3f\n", key.c_str(), value, m[key]);
	}

	for(auto mit=m.begin(); mit != m.end(); ++mit) {

		printf("%s\t%.2f\n", mit->first.c_str(), mit->second);
		//std::cout << mit->first << "\t" << mit->second << "\n";
	}

	return 0;
}
