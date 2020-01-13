#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

typedef long int int64;
typedef void (*codeptr)();
typedef unsigned char uchar;


typedef struct {int64 hp; bool prim; } dict_s;
dict_s dict[256];
char lwc; // last work created
char heap[1000];
int hptr = 0;

int ip;

int sstack[100];
int sp = 0;
void spush(int v) { sstack[sp++] = v; }
int spop() {return sstack[--sp]; }

int rstack[100];
int rp = 0;
void rpush(int v) { rstack[rp++] = v; }
int rpop() {return rstack[--rp]; }

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
	//puts("Finished definition");

}

void run(char ch)
{
	dict_s d = dict[ch];
	int ip0 = ip = d.hp;
	while(1) {
		ch = heap[ip++];
		if(ch == ';') break;
		if(ch == 'x') {
		       if(spop()) 
			       break;
		       else
			       continue;
		}
		if(ch == 'a') { ip = ip0; continue; }
		d = dict[ch];
		if(d.prim) {
			codeptr fn = (codeptr) d.hp;
			fn();
		} else {
			rpush(ip);
			run(ch);
			ip = rpop();
		}
	}
	//puts("Finished running");



}


void p_key() { spush(getchar()); }
void p_emit() { putchar(spop()); }
void p_push_lit_char() { spush(heap[ip]); rstack[rp-1] = ++ip;} 
void p_minus() { spush(-spop() + spop()); }
void p_dup()  { int v = spop(); spush(v); spush(v); }
void p_not()	{ if(spop()==0) spush(1); else spush(0); }

typedef struct {char name; void* fn; } prim_s;
prim_s prims[] = {
	{'~', p_not},
	{'d', p_dup},
	{'-', p_minus},
	{'c', p_push_lit_char},
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
				run(lwc);
				break;
			case 'h': // halt
				goto finis;
				break;
		}
		//puts("ok");
	}
finis:
	puts("Halted");

	return 0;
}
