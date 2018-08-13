#include <cstdint>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

typedef vector<uint8_t> bytes_t;


std::string slurp(std::ifstream& in) {
    std::stringstream sstr;
    sstr << cin.rdbuf();
    return sstr.str();
}


/*
chars_t
readFile2(const string &fileName)
{
    //ifstream ifs(fileName.c_str(), ios::in | ios::binary | ios::ate);
    ifstream ifs(std::cin, ios::in | ios::binary | ios::ate);

    ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, ios::beg);

    chars_t bytes(fileSize);
    ifs.read(bytes.data(), fileSize);

    ifs.close();
    return bytes;
    //return string(bytes.data(), fileSize);
}
*/

int main()
{
    std::stringstream sstr;
    sstr << cin.rdbuf();
    string str = sstr.str();
    for(auto c: str) {
	    int c1 = (uint8_t) c;
	    cout << c1 << "\n";
}
    //return sstr.str();
	//string str = slurp(cin);
	/*
	chars_t cs = readFile2("dunno");
	for(auto c:cs) {
		cout << c << "\n";
	}
	*/

}
