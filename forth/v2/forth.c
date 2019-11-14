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
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef intptr_t cell_t;
typedef uint8_t ubyte;
typedef void (*codeptr)();

ubyte heap[10000];
ubyte* hptr = heap;

bool compiling = false;
bool show_prompt = true;

//char _TIB[136]; // The input buffer
//char* TIB = _TIB;
char tib[132];
int bytes_read = 0; // number of bytes read into TIB


typedef struct { // dictionary entry
	// name of word is packed before this struct
	void* prev;
	ubyte  flags;
	char* name;
	//byte  len; 
	//char  name[]; // the name will actually be longer
} dent_s;

dent_s *latest = NULL; // latest word being defined

const ubyte F_IMM = 1;

/* not sure if this is strictly necessary
 * because we use strcasecmp instead of strcmp
 */
char* strupr(char* str) 
{ 
	int c = -1, i =0;
	if(!str) return NULL;
	//char* ptr = str;
	while(c = toupper(str[i])) {
		str[i] = c;
		i++;
		if(c==0) return str;
	}
	return str;
}


void undefined(char* token){
	printf("undefined word:<%s>\n", token);
}

cell_t dref(void* addr) { return *(cell_t*)addr; }

void store (cell_t pos, cell_t val) { *(cell_t*)pos = val; }

void heapify (cell_t v)
{
	store((cell_t)hptr, v);
	hptr += sizeof(cell_t);
}

void create_header(ubyte flags, char* zname)
{
	char* name = hptr;
	//do { *hptr++ = toupper(*zname) } while( *zname++); // include trailing 0
	for(int i = 0 ; i<= strlen(zname); ++i) *hptr++ = toupper(zname[i]); // include trailing 0
	dent_s dw;
	dw.prev = latest;
	dw.flags = flags;
	dw.name = name;
	memcpy(hptr, &dw, sizeof(dw));
	latest = (dent_s*) hptr;
	hptr += sizeof(dw);

	/*
	   memcpy(hptr, &latest, sizeof(void*));
	   latest = (dent_s*) hptr;
	   hptr += sizeof(void*);

	 *hptr++ = flags;
	 *hptr++ = strlen(zname);
	 for(int i = 0 ; i< strlen(zname); ++i) *hptr++ = toupper(zname[i]);
	 */
	//printf("createz heapifying fn %p at %p\n", fn, hptr);
}

void createz(ubyte flags, char* zname, codeptr fn) // zname being a null-terminated string
{
	create_header(flags, zname);
	heapify((cell_t)fn);
}

codeptr xt_find(char* name, ubyte* flags)
{
	dent_s* dw = latest;
	//strupr(name);
	while(dw) {
		if(strcasecmp(name, dw->name) == 0)
			goto found;
		dw = dw->prev;
	}
	return NULL;
found:
	puts("found word");
	//codeptr xt = (codeptr) dref((void*)dw+sizeof(dw)-(void*)heap);
	codeptr xt = (codeptr) dref(++dw);
	return xt;
}



char* token;
char* rest;
char* delim_word (char* delims, bool upper)
{
	token = strtok_r(rest, delims, &rest);
	if(upper) strupr(token);
	return token;
}

char* word () { delim_word(" \t\n", true); }



void process_tib();


void p_hello() {
	puts("hello world");
}

void p_words() {
	dent_s* dw = latest;
	while(dw) {
		puts(dw->name);
		//for(int i = 0; i < dw->len; ++i) putchar(dw->name[i]);
		//puts("");
		dw = dw->prev;
	}
}

typedef struct {ubyte flags; char* zname; codeptr fn; } prim_s;
prim_s prims[] =  {
	{0,	"WORDS", p_words},
	{0,	"HELLO", p_hello},
	0
};

void add_primitives()
{
	prim_s* p = prims;
	while(p->zname) {
		//puts(p->zname);
		createz(p->flags, p->zname, (codeptr) p->fn);
		p++;
	}
}

void add_derived()
{
	// TODO
}

void process_token(char* token)
{
	//dent_s* dw = dw_find(token);
	ubyte flags;
	codeptr xt = xt_find(token, &flags);
	if(xt == 0) {
		/* TODO
		   cell_t v;
		   if(int_str(token, &v)) {
		   if(compiling)
		   embed_literal(v);
		   else
		   push(v);
		   } else {
		   undefined(token);
		   }
		   */
	} else {
		//codeptr xt = (codeptr) dref(dw+sizeof(dw));
		if(compiling && !(flags & F_IMM))
			heapify((cell_t)xt);
		else
			xt();
		//xdw(dw);
	}
}
void process_tib()
{
	rest = tib;
	while(word()) process_token(token);
}
int main()
{
	assert(sizeof(size_t) == sizeof(cell_t));
	compiling = false;
	add_primitives();
	add_derived();

	puts("words are");
	//char * word = "WORDS";
	process_token("words");
	//p_words();
	puts("fin");

	while(fgets(tib, sizeof(tib), stdin)) {
		process_tib();
		if(show_prompt) puts("  ok");
	}
	return 0;
}
