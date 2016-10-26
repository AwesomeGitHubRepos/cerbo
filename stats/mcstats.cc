#include <math.h>
#include <map>
#include <vector>
#include "mcstats.h"


/** sort doubles in place */
void sortd (doubles &xs)
{
	//vecvec_t rows = read_registered_dsv(etransa);
	//vector<double> res;


	// cubie seems to have problems with sorting, so I'll write my own algo: insertion sort
	//                         // https://en.wikipedia.org/wiki/Insertion_sort

	for(int i=0; i< xs.size(); i++) {

		double x = xs[i];
		int j = i-1;
		while(j>=0 && xs[j] > x) {
			xs[j+1] = xs[j];
			j -= 1;
		}
		xs[j+1] = x;
	}



	//for(auto &e : res) { std::cout << e.sym << "\t" << e.dstamp << std::endl; }

	//return res;
}

/** 
 calculates quantiles according to Excel interpolation formula
 https://en.wikipedia.org/wiki/Quantile
 assumes array is sorted
*/
double quantile(const doubles &arr, double q)
{
	double res;
	int len = arr.size();
	double hd = (len-1) * q ; //+ 1;
	int hi = floor(hd);	
	if(q == 1) res = arr[len-1];
	else res = arr[hi] + (hd - hi) * (arr[hi+1] - arr[hi]);
	return res;

}				

void basic_stats(const doubles &ds, stats &s)
{
	double sxx =0, sx = 0;
	for(double d: ds) { sx += d; sxx += d*d; };
	int n = ds.size();
	s.n = n;
	s.mean = sx / n;
	s.stdev = sqrt( (n*sxx - sx*sx)/n/(n-1));
}


/**
 * calculate the fractional rank of a vector of doubles
 * https://en.wikipedia.org/wiki/Ranking#Ranking_in_statistics
 */
std::vector<double> frank(const std::vector<double>& arr)
{
	// count the occurances
	std::map<double, double> m;
	for(auto x: arr) {
		auto it = m.find(x);
		if(it == m.end()) {m[x] = 1; } else { m[x] += 1;}
	}

	//attach fractional ranks to occurances	
	double rank = 0;
	for(auto it = m.begin(); it != m.end(); ++it) {
		// derived from https://www.mathsisfun.com/algebra/sequences-sums-arithmetic.html :
		double rnk = rank + 0.5 + it->second / 2.0 ;
		rank += it->second ;
		it->second = rnk;
	}

	std::vector<double> res;
	for(auto x: arr)  res.push_back(m[x]);
	return res;
}	
