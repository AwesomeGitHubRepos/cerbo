/* deliberately engineer a core dump
*/
#include <stdio.h>

void foo(int i) {
	int j;
	j = 1/ i; // should crash here
	printf("%d\n", j);
}

void main() {
	foo(0);
}
