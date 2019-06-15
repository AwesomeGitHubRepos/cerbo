#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
//#include <ctype.h>
//#include <cstddef>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef size_t cell_t;
//static_assert(sizeof(cell_t) == sizeof(char*));
typedef unsigned char byte;

size_t IP;	// Interpreter Pointer. 

cell_t pstack[10];
cell_t *PSP = pstack; // Parameter Stack, aka data stack, aka SP
#define PUSH(x) *PSP++ = (cell_t) x // push an item onto the parameter stack
#define POP()   *--PSP //pop an item off the parameter stack

size_t RSP[10]; // Return Stack Pointer, aka RP
uint8_t heap[10000];
uint8_t* hptr = heap;	// User Pointer. Holds the base address of the task's user area
size_t W;	// Working register.  Multi-purpose
size_t X;	// Working register


char _TIB[136]; // The input buffer
char* TIB = _TIB;
int bytes_read = 0; // number of bytes read into TIB


typedef struct { // dictionary entry
	void* next;
	byte  len; // it is OR'd with flags
	char  name[]; // the name will actually be longer
} dent_s;

dent_s *dict = NULL, *latest = NULL; // pointers to first and last dictionary entry

char strupr(char c) { if('a' <= c && c <= 'z') return c-'a'+'A'; else return c; }

void createz(char* zname) // zname being a null-terminated string
{
	if(dict==NULL) dict = (dent_s*) hptr;

	if(latest) {
		latest->next = hptr;
	}

	latest = (dent_s*) hptr;
	cell_t nil = 0;
	memcpy(hptr, &nil, sizeof(void*));
	hptr += sizeof(void*);
	//printf("createz:%ld\n", strlen(zname));
	*hptr++ = strlen(zname);
	for(int i = 0 ; i< strlen(zname); ++i) *hptr++ = strupr(zname[i]);
}

/* leave the ASCII value for space (DEC 32) on the stack
 */
void p_bl() { PUSH(32); }

void p_find()
{
	_TIB[_TIB[0]+1] = 0;
	printf("TODO: finding:%s", TIB+1);
}

/* read the next work from stdin and store it on the stack as address.
 * The delimiter is stored in the PAD.
 * The first char stores the number of characters, excluding the
 * delimiter.
 *
 * */
char wordpad[32];
void p_word()
{
// TODO check word len overflow
	int count = 0, c;

	bool skip_spaces = true;
	for(;;) {
		int c = getchar();
		if((c == ' ') && skip_spaces) continue;
		skip_spaces = false;
		wordpad[++count] = c;
		if(c == '\n' || c == ' ' || c == EOF || c == 0) {count--; break; }
	}
	wordpad[0] = count;
}



void p_nextword () { p_bl(); p_word(); p_find(); }

void p_words() {
	dent_s* dw = dict;
	while(dw) {
		for(int i = 0; i < dw->len; ++i) putchar(dw->name[i]);
		puts("");
		dw = dw->next;
	}
	// TODO
}

void p_dots()
{
	puts("TODO p_dots");
}

typedef struct {char* zname; void (*fn)(); } prim_s;
prim_s prims[] =  {
	{"WORDS", p_words},
	{".S", p_dots},
	0
};
void add_primitives()
{
	prim_s* p = prims;
	while(p->zname) {
		//p->len = strlen(p->zname);
		printf("strlen=%ld\n", strlen(p->zname));
		createz(p->zname);
		p++;
	}
}

int main()
{
	int8_t STATE = 0; // 0 means interpretting
	add_primitives();
	p_words(); // TODO remove
	for(;;) {
		p_word();

		printf("word is:<");
		for(int i = 1; i <= wordpad[0]; ++i) printf("%c", wordpad[i]); // debug purposes
		printf(">\n");

		if(wordpad[wordpad[0]+1] == '\n') puts("  ok");
	}
	return 0;
}
