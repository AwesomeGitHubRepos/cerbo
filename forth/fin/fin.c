#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

typedef long int int64;
typedef void (*codeptr)();


typedef struct {int64 hp; bool prim; } dict_s;
dict_s dict[256];
char lwc; // last work created
char heap[1000];
int hptr = 0;

void define()
{
	int ch = getchar();
	lwc = ch;
	dict[ch].hp = hptr;
	dict[ch].prim = false;

	while(ch = getchar()) {
		heap[hptr++] = ch;
		if(ch== ';') break;
	}
	puts("Finished definition");

}

void run()
{
	puts("Running");
	char ch = lwc;
	//static hp_end = dict
	dict_s d = dict[ch];
	int ip = d.hp;
	while(1) {
		ch = heap[ip++];
		if(ch == ';') break;
		d = dict[ch];
		if(d.prim) {
			codeptr fn = (codeptr) d.hp;
			fn();
		}
		// TODO
	}
	puts("Finished running");



}


void p_key()
{
	puts("key called");
}


void p_emit()
{
	puts("emit called");
}

typedef struct {char name; void* fn; } prim_s;
prim_s prims[] = {
	{'k', p_key},
	{'e', p_emit},
	0
};

void add_prims()
{
	char ch;
	prim_s* p = prims;
	while(ch = p->name) {
		//char ch = p->name;
		dict[ch].hp = (int64) p->fn;
		dict[ch].prim = true;
		p++;
	}

}

int main()
{
	printf("int:%ld, void*:%ld\n", sizeof(int64), sizeof(void*));
	assert(sizeof(int64) >= sizeof(void*));

	add_prims();
	int ch;
	while(ch = getchar()) {
		if(ch == EOF) break;
		switch(ch) {
			case ':':
				define();
				break;
			case 'r':
				run();
				break;
			case 'h': // halt
				goto finis;
				break;
		}
		puts("ok");
	}
finis:
	puts("Halted");

	return 0;
}
