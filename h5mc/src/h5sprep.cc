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

class mat_c {
	public:
		mat_c(int nrows, int ncols);
		mat_c() {};
		string get(int r, int c) const{ return m_mat[idx(r,c)];} ;
		void set(int r, int c, string val) { m_mat[idx(r,c)] = val; };
	private:
		vector<string> m_mat;
		int m_rows = 0;
		int m_cols = 0;
		int idx(int r, int c) const { return r*m_cols + c; };
};


mat_c::mat_c(int nrows, int ncols) : m_rows(nrows), m_cols(ncols)
{
	//m_rows = nrows;
	//m_cols = ncols;
	m_mat.resize(nrows*ncols);
	//cout << "num cells: " << m_rows << "x" << m_cols << ": " <<m_mat.size() << endl;
}


typedef struct {
	strings raw_hdr_fields;
	strings cooked_hdr_fields;
	strmat  datamat;
	mat_c mat;
	int nrows;
	int ncols;
} tbl_s;


const string root = "/home/mcarter/.fortran/STATSLIST";

tbl_s
read_bad_csv()
{
	tbl_s tbl;

	int r,c;
	string raw = slurp(root + "/StatsList.csv");
	strings rows = split(raw, 13); // ^M

	// process header
	tbl.raw_hdr_fields = split(rows[0], ',');
	tbl.ncols = tbl.raw_hdr_fields.size();
	for(c=0; c<tbl.ncols; ++c) {
		string field_name = tbl.raw_hdr_fields[c];
		if(field_name.substr(0,2) == "F.")
			field_name = field_name.substr(2);
		tbl.cooked_hdr_fields.push_back(field_name);
	}

	tbl.nrows = rows.size() -1; 
	//string field_names[tbl.ncols];
	for(int i=1; i< rows.size(); ++i){
		string s = rows[i];
		strings strs = split(s, ',');
		tbl.datamat.push_back(strs);
	}


	//string mat[tbl.nrows][tbl.ncols];
	tbl.mat = mat_c(tbl.nrows, tbl.ncols);
	for(c=0; c< tbl.ncols; ++c){
		for(r=0; r<tbl.nrows; ++r){
			string s = tbl.datamat.at(r).at(c);
			s = trim(s, " \r\t\n\"");
			if(s.size() ==0) s = "nan";
			tbl.mat.set(r, c,  s);
		}
	}

	return tbl;
}

ofstream get_out(string filename)
{
	//ofstream ofs;
	const string filename1 = root + "/" + filename; 
	ofstream ofs(filename1.c_str(), std::ofstream::out);
	return ofs;
}
void
make_recfile(const tbl_s& tbl)
{
	ofstream ofs = get_out("StatsList.rec");
	//string filename = "/home/mcarter/.fortran/STATSLIST/StatsList.rec";
	//ofstream ofs(filename.c_str(), std::ofstream::out);
	for(int r=0; r<tbl.nrows; ++r) {
		for(int c=0; c<tbl.ncols;++c) {
			ofs << tbl.cooked_hdr_fields[c] << ": " 
				<< tbl.mat.get(r, c)<< endl;
		}
		ofs << endl;
	}
	ofs.close();
}


void
make_cooked_csv(const tbl_s& tbl)
{

	// output rectified csv
	//string filename = "/home/mcarter/.fortran/STATSLIST/StatsList-1.csv";
	ofstream ofs = get_out("StatsList-1.csv");
	//ofs.open(filename.c_str(), std::ofstream::out);

	for(int c=0; c<tbl.ncols-1; ++c)
		ofs << tbl.cooked_hdr_fields[c] << ",";
	ofs << tbl.cooked_hdr_fields[tbl.ncols-1] << endl;


	for(int r=0; r<tbl.nrows; ++r) {
		for(int c=0; c<tbl.ncols-1;++c) {
			ofs << tbl.mat.get(r, c) << ",";
		}
		ofs << tbl.mat.get(r, tbl.ncols-1) << endl;
	}
	ofs.close();
}

void
create_columns(const tbl_s& tbl)
{
	// create individual columns
	string outdir = root + "/cols";
	supo::ssystem("mkdir -p " + outdir + "; rm -f " + outdir + "/*", true);
	for(int c=0; c<tbl.ncols; ++c){
		string filename = outdir + "/" + tbl.cooked_hdr_fields[c];
		ofstream ofs;
		ofs.open(filename.c_str());
		for(int r=0; r<tbl.nrows; ++r){
			ofs << tbl.mat.get(r, c) << endl;
		}
		ofs.close();
	}
}

int main()
{
	tbl_s tbl = read_bad_csv();

	make_recfile(tbl);
	make_cooked_csv(tbl);
	create_columns(tbl);

	return 0;
}
