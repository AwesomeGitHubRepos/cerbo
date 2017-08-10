#include <iostream>
#include <string>
#include <regex>
#include <iterator>
#include <vector>

int main()
{
    std::string str = " hello how are 2 * 3 you? 123 4567867*98";

    // use std::vector instead, we need to have it in this order
    std::vector<std::pair<std::string, std::string>> v
    {
        {"[0-9]+" , "NUMBERS"} ,
        {"[a-z]+" , "IDENTIFIERS"},
        {"\\*|\\+", "OPERATORS"}
    };

    std::string reg;

    for(auto const& x : v)
        reg += "(" + x.first + ")|"; // parenthesize the submatches

    reg.pop_back();
    std::cout << reg << std::endl;

    std::regex re(reg);
    auto words_begin = std::sregex_iterator(str.begin(), str.end(), re);
    auto words_end = std::sregex_iterator();

    for(auto it = words_begin; it != words_end; ++it)
    {
        size_t index = 0;

        for( ; index < it->size(); ++index)
            if(!it->str(index + 1).empty()) // determine which submatch was matched
                break;

        std::cout << it->str() << "\t" << v[index].second << std::endl;
    }

    return 0;
}
