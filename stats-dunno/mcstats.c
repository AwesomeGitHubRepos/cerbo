#include <alloca.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>

//#include <gsl/gsl_math.h>
//#include <gsl/gsl_statistics.h>

#include "mcstats.h"


void exp_fit_y(double *y, int n)
{
  int i;
  double *x;
  x = alloca(n * sizeof(double));
  assert(x);
  for(i =0; i<n; i++)
    {
      x[i] = i;
      y[i] = log(y[i]);
    }
  double a, b;
  least_squares(x, y, n, &a, &b);
  double r = exp(b);
  pstat("fexp", r);
}



/* compute a and b least squares y = a + b x
   http://www.efunda.com/math/leastsquares/lstsqr1dcurve.cfm
   TODO check calcs of least_squares()
*/
void least_squares(double *x, double *y, int n, double *a , double *b)
{
  int i;
  double sx = 0.0;
  double sxx = 0.0;
  double sxy = 0.0;
  double sy = 0.0;
  for(i = 0; i< n; i++)
    {
      sx += x[i];
      sxx += x[i] * x[i];
      sxy += x[i] * y[i];
      sy += y[i];
    }

  double t1 = n * sxx - sx * sx ;
  *a = (sy * sxx - sx * sxy)/ t1;
  *b = (n * sxy - sx * sy)/t1;
}


/* sample standard deviation 
Calculations verified on 05-May-2014
*/
double sstdev(double *x, int n)
{
	int i;
	double sx = 0.0f, sxx = 0.0f;
	for(i = 0; i <n; i++) {
		sx = sx + x[i];
		sxx = sxx + x[i] * x[i];
	}
	double a = n * sxx - sx * sx;
	return sqrt(a/n/(n-1));
}

void pstat(char *name, double value)
{
  printf("%s: %g\n", name, value);
}

