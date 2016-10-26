#ifndef MCSTATS_H
#define MCSTATS_H

#include <vector>

typedef std::vector<double> doubles;


struct stats_s { double n, sxx, mean, stdev /* sample */ /* sample */ /* sample */ /* sample */ /* sample */ /* sample */ /* sample */ /* sample */ ; }; 
typedef struct stats_s stats;

void sortd (doubles &xs);
double quantile(const doubles &arr, double q);
void basic_stats(const doubles &ds, stats &s);
std::vector<double> frank(const std::vector<double>& arr);

#endif
