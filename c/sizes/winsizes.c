/****************************************************************************
 *                                                                          *
 * File    : winsizes.c                                                         *
 *                                                                          *
 * Purpose : try to figure out the sizes and types of windows
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

/* 14-aug-2011 mcarter started */


#include <stdio.h>
#include <stdlib.h>

#include <windows.h>


void PrintVal(char * str, int size) {printf("%-10s = %d\n", str, size);}
#define PRSIZE(x) PrintVal(#x, sizeof(x));
#define PRVAL(x)  PrintVal(#x, (long) x);


int main(int argc, char *argv[])
{

	puts("Sizes of various data items in bytes follows:");

	PRSIZE(char *);

        PRVAL(CW_USEDEFAULT);

	PRSIZE(double);
        PRVAL(FALSE);
	PRSIZE(float);

        PRSIZE(HICON);

        PRVAL(IDC_ARROW);

        PRVAL(IDI_APPLICATION);

	PRSIZE(int);
	PRSIZE(long int);

        PRVAL(MB_ICONEXCLAMATION);
        PRVAL(MB_OK);
        PRVAL(MB_OKCANCEL);

	PRSIZE(size_t);

        PRVAL(TRUE);

        PRVAL(WHITE_BRUSH);
        PRSIZE(WNDCLASSEX);

        PRVAL(WM_CLOSE);
        PRVAL(WM_DESTROY);

        PRVAL(WS_OVERLAPPEDWINDOW);


    return 0;
}

