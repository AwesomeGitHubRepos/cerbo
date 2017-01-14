/*
 * */

#include <cmath>
#include <map>
#include <iostream>
#include <fstream>
#include <locale.h>
#include <string.h>

#include "common.hpp"
//#include <supo_general.hpp>
#include "uprice.hpp"



using namespace std;
//using namespace supo;

void 
mkuprices(const detran_cs& the_etrans)
{
	string fname =  s3("uprice.rep");
	ofstream ofs;
	ofs.open(fname.c_str(), ofstream::out);
	for(const auto& e:the_etrans) {
		const etran_c &e1 = e.etran;
		strings fields = strings {  
			supo::pad_right(e1.ticker, 7), 
				e1.dstamp, e.ucost.str(), e1.buystr() 
		};
		ofs << supo::intercalate(" ", fields) << endl;
	}
	ofs.close();

}
