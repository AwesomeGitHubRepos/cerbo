/*
 * I release this file into the Public Domain, together with
 * all the other files in this directory.
 *
 * Enjoy!
 *
 * Mark Carter, Nov 2019
 *
 */

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef intptr_t cell_t;

cell_t heap[10000];
cell_t* hptr = heap;

bool g_compiling = false;
bool g_show_prompt = true;

//char _TIB[136]; // The input buffer
//char* TIB = _TIB;
char tib[132];
int g_bytes_read = 0; // number of bytes read into TIB



void process_tib();




void add_primitives()
{
	// TODO
}

void add_derived()
{
	// TODO
}

void process_tib()
{
	// TODO
}
int main()
{
	assert(sizeof(size_t) == sizeof(cell_t));
	g_compiling = false;
	add_primitives();
	add_derived();
	while(fgets(tib, sizeof(tib), stdin)) {
		process_tib();
		if(g_show_prompt) puts("  ok");
	}
	return 0;
}
