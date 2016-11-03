#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>

#include <supo.hpp>

#include "common.hpp"

using namespace std;


const string root = "/home/mcarter/.fortran/STATSLIST";

void read_bad_csv(strings& hdr_fields, strmat& datamat)
{
	string raw = slurp(root + "/StatsList.csv");
	strings rows = split(raw, 13); // ^M

	hdr_fields = split(rows[0], ',');
	for(int i=1; i< rows.size(); ++i){
		string s = rows[i];
		strings strs = split(s, ',');
		datamat.push_back(strs);
	}

	
}

int main()
{
	strings hdr_fields;
	strmat datamat;
	read_bad_csv(hdr_fields, datamat);


	int r, nrows = datamat.size();
	int c, ncols = hdr_fields.size();
	string field_names[ncols];
	string mat[nrows][ncols];
	for(c=0; c< ncols; ++c){
		string field_name = hdr_fields[c];
		if(field_name.substr(0,2) == "F.")
			field_name = field_name.substr(2);
		field_names[c] = field_name;

		for(r=0; r<nrows; ++r){
			string s = datamat.at(r).at(c);
			s = trim(s, " \r\t\n\"");
			if(s.size() ==0) s = "nan";
			mat[r][c] = s;
		}
	}

	//std::ofstream ofs;
	string filename;

	//write_rec(field_names, datamat_out, nrows, ncols);
	filename = "/home/mcarter/.fortran/STATSLIST/StatsList.rec";
	ofstream ofs(filename.c_str(), std::ofstream::out);
	for(int r=0; r<nrows; ++r) {
		for(int c=0; c<ncols;++c) {
			ofs << field_names[c] << ": " 
				<< mat[r][c]<< endl;
		}
		ofs << endl;
	}
	ofs.close();

	// output rectified csv
	filename = "/home/mcarter/.fortran/STATSLIST/StatsList-1.csv";
	ofstream ofsc;
	ofsc.open(filename.c_str(), std::ofstream::out);

	for(c=0; c<ncols-1; ++c)
		ofsc << field_names[c] << ",";
	ofsc << field_names[ncols-1] << endl;


	for(r=0; r<nrows; ++r) {
		for(c=0; c<ncols-1;++c) {
			ofsc << mat[r][c] << ",";
			//cout << mat[r][c] << " ";
		}
		ofsc << mat[r][ncols-1] << endl;
		//cout << r << endl ;
	}
	ofsc.close();


	// create individual columns
	string outdir = root + "/cols";
	supo::ssystem("mkdir -p " + outdir + "; rm -f " + outdir + "/*", true);
	for(c=0; c<ncols; ++c){
		filename = outdir + "/" + field_names[c];
		ofs.open(filename.c_str());
		for(r=0; r<nrows; ++r){
			ofs << mat[r][c] << endl;
		}
		ofs.close();
	}

	return 0;
}
