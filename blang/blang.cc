#include <iostream>
#include <string>
#include <regex>
#include <iterator>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;

enum blang_t { T_NUL, T_NUM, T_ID, T_ASS, T_OP };

struct token { blang_t type; string value; } token;
typedef vector<struct token> tokens;

tokens tokenise(const string& str)
{
	tokens result;

	//std::string str = " hello how are 2 * 3 you? 123 4567867*98";

	// use std::vector instead, we need to have it in this order
	std::vector<std::pair<string, blang_t>> v
	{
		{"[0-9]+" , T_NUM} ,
			{"[a-z]+" , T_ID},
			{"="      , T_ASS},
			{"\\*|\\+", T_OP}
	};

	std::string reg;

	for(auto const& x : v)
		reg += "(" + x.first + ")|"; // parenthesize the submatches

	reg.pop_back();
	//std::cout << reg << std::endl;

	std::regex re(reg);
	auto words_begin = std::sregex_iterator(str.begin(), str.end(), re);
	auto words_end = std::sregex_iterator();

	for(auto it = words_begin; it != words_end; ++it)
	{
		size_t index = 0;

		for( ; index < it->size(); ++index)
			if(!it->str(index + 1).empty()) // determine which submatch was matched
				break;

		//std::cout << it->str() << "\t" << v[index].second << std::endl;
		//struct token toke = {.type = v[index].second, .value = it->str()};
		struct token toke{v[index].second, it->str()};
		result.push_back(toke);
	}

	return result;

}

int main()
{
	std::string str = R"(
for i = 1 to 6
	print i * 2
next
)";
	tokens tokes = tokenise(str);
	for(const auto& t: tokes) 
		cout <<  t.type << "\t" << t.value <<"\n";
	return 0;
}
