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

typedef struct {
	int size;
	cell_t contents[10];
} stack_t;

stack_t rstack = { .size = 0 }; // return stack
stack_t sstack = { .size = 0 }; // standard data stack

void push_x(stack_t* stk, cell_t v)
{
	//int* size = &stk->size;
	if(stk->size>=10)
		puts("Stack overflow");
	else
		stk->contents[stk->size++] = v;
}

cell_t pop_x(stack_t* stk)
{
	if(stk->size <=0) {
		puts("Stack underflow");
		return 0; // TODO maybe should throw
	} else 
		return stk->contents[--stk->size];
}
void push(cell_t v) { push_x(&sstack, v); }
cell_t pop(cell_t v)  { return pop_x(&sstack); }

void rpush(cell_t v) { push_x(&rstack, v); }
cell_t rpop(cell_t v)  { return pop_x(&rstack); }

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

bool int_str(char*s, cell_t *v)
{
	*v = 0;
	cell_t sgn = 1;
	if(*s=='-') { sgn = -1; s++; }
	if(*s == '+') s++;
	if(*s == 0) return false;
	while(*s) {
		if('0' <= *s && *s <= '9')
			*v = *v * 10 + *s - '0';
		else
			return false;
		s++;
	}
	*v *= sgn;
	return true;
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
	for(int i = 0 ; i<= strlen(zname); ++i) *hptr++ = toupper(zname[i]); // include trailing 0
	dent_s dw;
	dw.prev = latest;
	dw.flags = flags;
	dw.name = name;
	memcpy(hptr, &dw, sizeof(dw));
	latest = (dent_s*) hptr;
	hptr += sizeof(dw);
}

void createz(ubyte flags, char* zname, codeptr fn) // zname being a null-terminated string
{
	create_header(flags, zname);
	heapify((cell_t)fn);
}

codeptr xt_find(char* name, ubyte* flags) // name can be lowercase, if you like
{
	dent_s* dw = latest;
	codeptr xt = 0;
	//strupr(name);
	while(dw) {
		if(strcasecmp(name, dw->name) == 0) break;
		//goto found;
		dw = dw->prev;
	}
	//return NULL;
	//found:
	if(dw) xt = (codeptr) dref(++dw);
	return xt;
}

void heapify_word(char* name)
{
	ubyte flags;
	codeptr xt = xt_find(name, &flags);
	heapify((cell_t) xt);
}

void embed_literal(cell_t v)
{
	heapify_word("LIT");
	heapify(v);

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


void p_hi() {
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

void p_lit() 
{
	puts("TODO p_lit");
	/*
	   cell_t v = dref((void*) rstack[rtop-1]);
	   rstack[rtop-1] += sizeof(cell_t);
	   push(v);
	   */
}

void p_dots()
{
	printf("Stack: (%d):", sstack.size);
	for(int i = 0; i< sstack.size; ++i) printf("%ld ", sstack.contents[i]);
}

void docol()
{
	puts("TODO docol");
}

void p_semi()
{
	heapify_word("EXIT");
	compiling = false;
}

void p_colon()
{
	word();
	createz(0, token, docol);
	compiling = true;
}

void p_exit()
{
}

typedef struct {ubyte flags; char* zname; codeptr fn; } prim_s;
prim_s prims[] =  {
	{0,	"EXIT", p_exit},
	{0,	":", p_colon},
	{0,	";", p_semi},
	{0,	".S", p_dots},
	{0,	"LIT", p_lit},
	{0,	"WORDS", p_words},
	{0,	"HI", p_hi},
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

void eval_string(char* str)
{
	strncpy(tib, str, sizeof(tib));
	process_tib();
}

char* derived[] = {
	//"hi hi",
	0
};

void add_derived()
{
	char** strs = derived;
	while(*strs) {
		eval_string(*strs++);
	}

}

void process_token(char* token)
{
	//dent_s* dw = dw_find(token);
	ubyte flags;
	codeptr xt = xt_find(token, &flags);
	if(xt == 0) {
		cell_t v;
		if(int_str(token, &v)) {
			if(compiling)
				embed_literal(v);
			else
				push(v);
		} else {
			undefined(token);
		}
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

	/*
	   puts("words are");
	   process_token("words");
	   puts("fin");
	   */

	while(fgets(tib, sizeof(tib), stdin)) {
		process_tib();
		if(show_prompt) puts("  ok");
	}
	return 0;
}
