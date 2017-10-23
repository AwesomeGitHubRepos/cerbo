#include <cmath>
#include <map>
#include <iostream>
#include <fstream>
#include <locale.h>
#include <string.h>

#include "common.h"
#include "uprice.h"

using namespace std;

void 
mkuprices(const detran_cs& the_etrans)
{
	string fname =  s3("uprice.rep");
	ofstream ofs;
	ofs.open(fname.c_str(), ofstream::out);
	for(const auto& e:the_etrans) {
		//const etran_c &e1 = e.etran;
		strings fields = strings {  
			supo::pad_right(e.ticker, 7), 
				e.dstamp, e.ucost.wide(), e.buystr() 
		};
		ofs << supo::intercalate(" ", fields) << endl;
	}
	ofs.close();

}
