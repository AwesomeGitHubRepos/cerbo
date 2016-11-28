#include <algorithm>
#include <stdlib.h>
#include <istream>
#include <iostream>
#include <set>
#include <fstream>

#include <supo_general.hpp>
#include <supo_parse.hpp>

using namespace std;

int main()
{
	istream& is =  cin;
	//string s = "foo bar";
	//supo::strings ss = supo::tokenize_line(s);
	const supo::strmat m = supo::tokenize_stream(is);

	// filenames
	set<string> commands;
	for(const auto& ss:m) {
		//cout << ss[0] << endl;
		if(ss.size() < 2) continue;
		commands.insert(ss[0]);
	}

	for(const auto& c:commands) {
		string fname = c + ".csv";
		ofstream ofs;
		ofs.open(fname, ofstream::out);
		for(const auto& row:m) {
			if(row.size() < 2) continue;
			if(row[0] != c) continue;
			supo::strings rout;
			for(auto it = row.begin(); it < row.end(); ++it) {
				if(it == row.begin()) continue;
				ofs << *it;
				if(it < row.end()-1) ofs << ",";
//					cout << * it << endl;
			}
			ofs << endl;
			//copy(row.begin()+1, row.end(), rout.begin());
			//ofs << supo::intercalate(",", rout) << endl;
		}
		ofs.close();
	}
		//cout << c << "\n";

	//cout << "Finished\n" ;
	return EXIT_SUCCESS;
}
