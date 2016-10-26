#pragma once
#include <vector>

typedef std::vector<double> doubles;


typedef struct stats_t { 
	double n = 0, sxx = 0, mean = 0, sum = 0, stdev =0;
} stats_t;

void sortd (doubles &xs);
double quantile(const doubles &arr, double q);
stats_t basic_stats(const doubles &ds);
std::vector<double> frank(const std::vector<double>& arr);
