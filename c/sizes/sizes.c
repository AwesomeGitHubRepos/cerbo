/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Console mode (command line) program.                           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

/* program to print the sizes of things */
/* 06-aug-2006 mcarter started */


#include <stdio.h>
#include <stdlib.h>


void PrintSize(char * str, int size)
{
	printf("%-10s = %d\n", str, size);
}

int main(int argc, char *argv[])
{

	puts("Sizes of various data items in bytes follows:");

	PrintSize("char *", sizeof(char *));
	PrintSize("double ", sizeof(double));
	PrintSize("float ", sizeof(float));
	PrintSize("int", sizeof(int));
	PrintSize("long int", sizeof(long int));
	PrintSize("size_t", sizeof(size_t));


    return 0;
}

