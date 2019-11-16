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

//typedef intptr_t cell_t;
#if(__SIZEOF_POINTER__ ==8)
typedef int64_t cell_t;
#endif

typedef cell_t* cellptr;
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
cell_t pop()  { return pop_x(&sstack); }
#define STOP sstack.contents[sstack.size-1]
#define STOP_1 sstack.contents[sstack.size-2]

void rpush(cell_t v) { push_x(&rstack, v); }
cell_t rpop()  { return pop_x(&rstack); }
#define RTOP rstack.contents[rstack.size-1]

ubyte heap[10000];
ubyte* hptr = heap;
cellptr W; // cfa  of the word to execute

bool compiling = false;
bool show_prompt = true;
ubyte flags = 0;

char tib[132];
int bytes_read = 0; // number of bytes read into TIB


typedef struct { // dictionary entry
	// name of word is packed before this struct
	void* prev;
	ubyte  flags;
} dent_s;

dent_s *latest = NULL; // latest word being defined

const ubyte F_IMM = 1 << 7;


/* not sure if this is strictly necessary
 * because we use strcasecmp instead of strcmp
 */
char* strupr(char* str) 
{ 
	int c = -1, i =0;
	if(!str) return NULL;
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
	ubyte noff = 0; // name offset
	for(noff = 0 ; noff<= strlen(zname); ++noff) *hptr++ = toupper(zname[noff]); // include trailing 0
	dent_s dw;
	dw.prev = latest;
	dw.flags = flags | noff;
	memcpy(hptr, &dw, sizeof(dw));
	latest = (dent_s*) hptr;
	hptr += sizeof(dw);
}

void createz(ubyte flags, char* zname, cell_t acf) // zname being a null-terminated string
{
	create_header(flags, zname);
	heapify(acf);
}

char* name_dw(dent_s* dw)
{
	char* str = (char*) dw;
	int name_off =  dw->flags & 0b111111;
	str -= name_off; 
	return str;

}

codeptr cfa_find(char* name) // name can be lowercase, if you like
{
	dent_s* dw = latest;
	//strupr(name);
	while(dw) {
		if(strcasecmp(name, name_dw(dw)) == 0) break;
		dw = dw->prev;
	}
	if(!dw) return 0;
	flags = dw->flags;
	dw++;

	return (codeptr) dw;
}

void heapify_word(char* name)
{
	codeptr xt = cfa_find(name);
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


void p_hi() { puts("hello world"); }

void p_words() {
	dent_s* dw = latest;
	while(dw) {
		puts(name_dw(dw));
		dw = dw->prev;
	}
}

void p_lit() 
{
	cell_t v = dref((void*)RTOP);
	RTOP += sizeof(cell_t);
	push(v);
}

void p_dots()
{
	printf("Stack: (%d):", sstack.size);
	for(int i = 0; i< sstack.size; ++i) printf("%ld ", sstack.contents[i]);
}


void p_plus() { push(pop() + pop()); }
void p_minus() { cell_t v1 = pop(), v2 = pop(); push(v2-v1); }
void p_mult() { push(pop() * pop()); }
void p_div() { cell_t v1 = pop(), v2 = pop(); push(v2/v1); }

void p_dot() { printf("%ld ", pop()); }

void p_tick()
{
	word();
	codeptr cfa = cfa_find(token);
	if(cfa)
		push((cell_t) cfa);
	else
		undefined(token);
}

void execute(codeptr cfa)	
{
	W = (cellptr) cfa;
	codeptr fn = (codeptr) dref(cfa);
	fn();
}
void p_execute()
{
	execute((codeptr) pop());
}

char* name_cfa(cellptr cfa)
{
	dent_s* dw = (dent_s*) cfa;
	return name_dw(--dw);
}

void docol()
{
	static codeptr cfa_exit = 0;
	if(cfa_exit==0) cfa_exit = cfa_find("EXIT");
	codeptr cfa;
	cellptr IP = W;
	IP++;
	for(;;) {
		cfa = (codeptr) dref(IP++);
		if(cfa == cfa_exit) break;
		rpush((cell_t)IP);
		execute(cfa);
		IP = (cellptr) rpop();
	}

}

void p_semi()
{
	heapify_word("EXIT");
	compiling = false;
}

void p_colon()
{
	word();
	createz(0, token, (cell_t) docol); 
	compiling = true;
}

void p_exit() { }

void p_at () { push(dref((void*)pop())); }
void p_exc() { cell_t pos = pop(); cell_t val = pop(); store(pos, val); }

void _create() { push((cell_t)++W); }
void p_create() { word(); createz(0, token, (cell_t) _create); }
void p_comma() { heapify(pop()); }
void p_prompt () { show_prompt = (bool) pop(); }

void p_lsb() { compiling = false; }
void p_rsb() { compiling = true; }
void p_here () { push((cell_t)hptr); }

void p_qbranch()
{
	if(pop()) 
		RTOP = dref((void*) RTOP);
	else
		RTOP += sizeof(cell_t);
}


void p_dup()
{
	cell_t v = pop();
	push(v); 
	push(v);
}

void p_z_slash () { delim_word("\"", false); push((cell_t)token); }
void p_emit() { printf("%c", (char)pop()); }
void p_swap() { cell_t temp = STOP; STOP = STOP_1; STOP_1 = temp; }
void p_immediate() { latest->flags |= F_IMM; }
void p_type () { printf("%s", (char*) pop()); }

void p_0branch()
{
	if(!pop()) 
		RTOP = dref((void*) RTOP);
	else
		RTOP += sizeof(cell_t);
}


void p_compile()
{
	cell_t cell = dref((void*)RTOP);
	heapify(cell);
	RTOP += sizeof(cell_t);
}


void p_fromr()
{
	cell_t tos = rpop();
	cell_t v = RTOP;
	RTOP = tos;
	push(v);
}

void p_tor()
{
	cell_t v = pop();
	cell_t temp = RTOP;
	RTOP = v;
	rpush(temp);
}

void p_branch()
{
	RTOP = dref((void*) RTOP);
}


typedef struct {ubyte flags; char* zname; codeptr fn; } prim_s;
prim_s prims[] =  {
	{0, 	"BRANCH", p_branch},
	{0,	">R", p_tor},
	{0,	"R>", p_fromr},
	{0, 	"TYPE", p_type},
	{0, 	"COMPILE", p_compile},
	{0, 	"0BRANCH", p_0branch},
	{0, 	"IMMEDIATE", p_immediate},
	{0, 	"SWAP", p_swap},
	{0, 	"EMIT", p_emit},
	{0, 	"Z\"", p_z_slash},
	{0,	 "DUP", p_dup},
	{F_IMM,	"[", p_lsb},
	{0, 	"]", p_rsb},
	{0, 	"?BRANCH", p_qbranch},
	{0, 	"HERE", p_here},
	{0, 	"PROMPT", p_prompt},
	{0, 	",", p_comma},
	{0, 	"CREATE", p_create},
	{0, 	"!", p_exc},
	{0, 	"@", p_at},
	{0,	"EXIT", p_exit},
	{0,	":", p_colon},
	{F_IMM,	";", p_semi},
	{0,	"EXECUTE", p_execute},
	{0,	"'", p_tick},
	{0,	".S", p_dots},
	{0,  	"+", p_plus},
	{0,  	"-", p_minus},
	{0,  	"*", p_mult},
	{0,  	"/", p_div},
	{0,  	".", p_dot},
	{0,	"LIT", p_lit},
	{0,	"WORDS", p_words},
	{0,	"HI", p_hi},
	0
};

void add_primitives()
{
	prim_s* p = prims;
	while(p->zname) {
		createz(p->flags, p->zname, (cell_t) p->fn);
		p++;
	}
}

void eval_string(char* str)
{
	strncpy(tib, str, sizeof(tib));
	process_tib();
}

char* derived[] = {
	": VARIABLE create 0 , ;",
	": 1+ 1 + ;",
	": CR 10 emit ;",
	": .\" z\" type ;",
	//": CONSTANT <builds , does> @ ;", // TODO reinstate
	": IF compile 0branch here 0 , ; immediate",
	": THEN here swap ! ; immediate",
	": ELSE compile branch here >r 0 , here swap ! r> ; immediate", 
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
	codeptr cfa = cfa_find(token);
	if(cfa == 0) {
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
		if(compiling && !(flags & F_IMM))
			heapify((cell_t)cfa);
		else
			execute(cfa);
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

	if(0) {
		//p_words();
		puts("words are");
		process_token("words");
		puts("fin");
	}

	while(fgets(tib, sizeof(tib), stdin)) {
		process_tib();
		if(show_prompt) puts("  ok");
	}
	return 0;
}
