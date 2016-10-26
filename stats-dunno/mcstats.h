#ifndef MCSTATS_H
#define MCSTATS_H

void exp_fit_y(double *y, int n);
void least_squares(double *x, double *y, int n, double *a , double *b);
void pstat(char *name, double value);
double sstdev(double *x, int n);

#endif // MCSTATS_H
