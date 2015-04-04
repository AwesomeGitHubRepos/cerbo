#include <gsl/gsl_math.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double get_d;
int get_i;
char get_s[80];

void get(FILE *fp)
{
	fgets(get_s, 80, fp);
	get_s[strlen(get_s)-1] = '\0';
	get_d = atof(get_s);
	get_i = atoi(get_s);
}
	
double vals[5000],  subset[1000];
char ftidx[5000][10];
int nvals, nsubs, i;

void process(char *desc)
{
	gsl_sort(subset, 1, nsubs);
	// following seems to cause a problem:
        // double m = gsl_stats_median_from_sorted_data(vals, 1, nvals);
        double m = (subset[(nsubs-1)/2] + subset[nsubs/2])/2.0f;
	printf("%7s %4d %10.1f\n", desc, nsubs, m);
}

main() 
{
	puts("Median PER by index");
	FILE *fp, *fp1; 
	fp = fopen("/home/mcarter/.fortran/_count", "r");
	get(fp);
	nvals = get_i;
	fclose(fp);

	
	fp = fopen("/home/mcarter/.fortran/PER", "r");
        fp1 = fopen("/home/mcarter/.fortran/FTSE_Index", "r");
	//nvals = get_d;
	for( i = 0; i< nvals; i++) {
		get(fp);
		vals[i] = get_d;
		get(fp1);

		strcpy(ftidx[i], get_s);
	}
	fclose(fp1);
	fclose(fp);

	
	nsubs = 0;
	for(i = 0; i< nvals; i++) {
		if (strcmp("FTSE100", ftidx[i]) == 0) { // FTSE250
			subset[nsubs] = vals[i];
			nsubs++;
		}
	}
	process("FTSE100");


	nsubs = 0;
        for(i = 0; i< nvals; i++) {
                if (strcmp("FTSE250", ftidx[i]) == 0) { // FTSE250
                        subset[nsubs] = vals[i];
                        nsubs++;
                }
        }
        process("FTSE250");



	nsubs = 0;
        for(i = 0; i< nvals; i++) {
                if (strcmp("FTSE100", ftidx[i]) != 0 && strcmp("FTSE250", ftidx[i]) != 0) { // FTSE250
                        subset[nsubs] = vals[i];
                        nsubs++;
                }
        }
        process("OTHER");



	memcpy(subset, vals, sizeof(double) * nvals);
	nsubs = nvals;
	process("ALL");
	/*
	gsl_sort(vals, 1, nvals);
	double m = gsl_stats_median_from_sorted_data(vals, 1, nvals);
	printf("%f %f\n", m, (vals[(nvals-1)/2] + vals[nvals/2])/2);

	
	for(i = 0; i < nvals; i++) {
		printf("Data %f\n", vals[i]);
	}*/
	
}
