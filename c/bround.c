/* rounding
   gcc bround.c -lm
 */
#include <math.h>
#include <stdio.h>



double bround(double x) 
{
  double c ,f, det;
  c = ceil(x);
  f = floor(x);

  // eliminate strange artifacts of creating "negative 0"
  if(c == 0) c = 0.0;
  if(f == 0) f = 0.0;
  // printf("c: %f ", c);


  det = c + f - 2.0*x;
  if (det < 0) {return c;}
  if (det > 0) {return f;}

  /* banker's tie */
 if(2*ceil(c/2) == c) {return c;} else {return f;};

}

double round2(double x) { return bround(x*100)/100; }

void show(double x)
{   
  printf("input: %lf, bround: %lf, round2: %lf\n", x, bround(x), round2(x));
}

void main() {
  show(12.3);
  show(12.5);
  show(12.7);
  show(13.33333);
  show(13.5);
  show(13.7);

  show(0.9);
  show(0.5);
  show(0.3);
  show(0.0);
  show(-0.9);
  show(-0.5);
  show(-0.3);



  show(-12.3);
  show(-12.5);
  show(-12.7777);
  show(-13.3);
  show(-13.5);
  show(-13.7);

}
