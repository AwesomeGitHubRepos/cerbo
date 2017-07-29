#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>

#include <supo_stats.h>

namespace supo {


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

double median(const doubles &arr)
{
	return quantile(arr, 0.5);
}

/** 
 calculates quantiles according to Excel interpolation formula
 https://en.wikipedia.org/wiki/Quantile
 assumes array is sorted
*/
double quantile(const doubles &arr, double q)
{
	double res;

	doubles sarr;
	for(auto a:arr) { if(!std::isnan(a)) sarr.push_back(a);}
	if(!std::is_sorted(begin(sarr), end(sarr)))
	       	std::sort(begin(sarr), end(sarr));

	int len = sarr.size();
	if(len == 0) return NAN;

	double hd = (len-1) * q ; //+ 1;
	int hi = floor(hd);	
	if(q == 1) res = sarr[len-1];
	else res = sarr[hi] + (hd - hi) * (sarr[hi+1] - sarr[hi]);
	return res;

}				

stats_t basic_stats(const doubles &ds)
{
	stats_t s;
	//double sxx =0, sx = 0;
	for(double d:ds) { s.sum += d; s.sxx += d*d; };
	//int n = ds.size();
	s.n = ds.size();
	s.mean = s.sum / s.n;
	s.stdev = sqrt( (s.n*s.sxx - s.sum*s.sum)/s.n/(s.n-1));
	return s;
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

} // namespace supo
