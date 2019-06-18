#include <assert.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <ctype.h>
//#include <cstddef>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if(__x86_64 ==1)
typedef int64_t cell_t;
#endif

//static_assert(sizeof(cell_t) == sizeof(char*));
typedef uint8_t byte;
typedef void (*codeptr)();

cell_t* IP;	// Interpreter Pointer. 
cell_t* wptr;

cell_t dstack[10]; // data stack
int tos = 0; // items on stack
//cell_t *PSP = pstack; // Parameter Stack, aka data stack, aka SP
void push(cell_t v)
{
	// TODO check for stack in range
	dstack[tos++] = v;
}
cell_t pop()
{
	// TODO check for stack in range
	//puts("pop() called");
	return dstack[--tos];
}
//#define PUSH(x) *PSP++ = (cell_t) x // push an item onto the parameter stack
//#define POP()   *--PSP //pop an item off the parameter stack

cell_t  rstack[10]; // Return Stack Pointer, aka RP
int rtop; // items on the stack
void rpush(cell_t v)
{
	rstack[rtop++] = v;
}
cell_t rpop()
{
	return rstack[--rtop];
}


uint8_t heap[100000];
uint8_t* hptr = heap;	// User Pointer. Holds the base address of the task's user area
size_t W;	// Working register.  Multi-purpose
size_t X;	// Working register

bool compiling = false; // start of by interpretting

char _TIB[136]; // The input buffer
char* TIB = _TIB;
int bytes_read = 0; // number of bytes read into TIB


typedef struct { // dictionary entry
	void* prev;
	byte  flags;
	byte  len; // it is OR'd with flags
	char  name[]; // the name will actually be longer
} dent_s;

dent_s *latest = NULL; // latest word being defined

const byte F_IMM = 1;
const byte F_SYN = 2; // a synthesised word, i.e. one that's a colon-definition


void execute(codeptr fn);
dent_s* find(char* name);
void docol();
void xdw(dent_s* dw);
void eval_string(char* str);
void process_tib();
void process_word();

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
}

typedef enum {
    STR2INT_SUCCESS,
    STR2INT_OVERFLOW,
    STR2INT_UNDERFLOW,
    STR2INT_INCONVERTIBLE
} str2int_errno;

/*
// from https://stackoverflow.com/questions/7021725/how-to-convert-a-string-to-integer-in-c#7021750
str2int_errno str2int(int *out, char *s, int base) {
    char *end;
    if (s[0] == '\0' || isspace(s[0]))
        return STR2INT_INCONVERTIBLE;
    errno = 0;
    long l = strtol(s, &end, base);
    if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX))
        return STR2INT_OVERFLOW;
    if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN))
        return STR2INT_UNDERFLOW;
    if (*end != '\0')
        return STR2INT_INCONVERTIBLE;
    *out = l;
    return STR2INT_SUCCESS;
}
*/


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

cell_t dref(void* addr)
{
	return *(cell_t*)addr;
}

void store(cell_t pos, cell_t val) { *(cell_t*)pos = val; }

void heapify (cell_t v)
{
	//*(cell_t*)hptr = v;
	store((cell_t)hptr, v);
	hptr += sizeof(cell_t);
}

void heapify_dw(dent_s* dw)
{
	*(cell_t*)hptr = (cell_t)dw;
	hptr += sizeof(void*);
}

void heapify_word(char* name)
{
	dent_s* dw = find(name);
	heapify_dw(dw);
	//*(dent_s*) hptr = dw;
	//hptr += sizeof(void*);
}
void create_header(byte flags, char* zname)
{
	memcpy(hptr, &latest, sizeof(void*));
	latest = (dent_s*) hptr;
	hptr += sizeof(void*);

	*hptr++ = flags;
	*hptr++ = strlen(zname);
	for(int i = 0 ; i< strlen(zname); ++i) *hptr++ = toupper(zname[i]);
	//printf("createz heapifying fn %p at %p\n", fn, hptr);
}
void createz(byte flags, char* zname, codeptr fn) // zname being a null-terminated string
{
	create_header(flags, zname);
	heapify((cell_t)fn);
}


/* leave the ASCII value for space (DEC 32) on the stack
 */
void p_bl() { push(32); }

dent_s* find(char* name)
{
	dent_s* dw = latest;
	int len = strlen(name);
	while(dw) {
		if(dw->len == len && strncmp(dw->name, name, len) == 0)
			return dw;
		dw = dw->prev;
	}
	return NULL;
}
void p_find ()
{
	_TIB[_TIB[0]+1] = 0;
	printf("TODO: finding:%s", TIB+1);
}


// return the position on the heap where the code begins
void* code (dent_s* dw)
{
	void* ptr = dw->name + dw->len;
	codeptr fn = *(codeptr*) ptr;
	if(fn == docol) {
		IP = ptr;
	}
	return (codeptr) ptr;
}

/* read the next work from stdin and store it on the stack as address.
 * The delimiter is stored in the PAD.
 * The first char stores the number of characters, excluding the
 * delimiter.
 *
 * */
char tib[132];
char* token;
char* rest;
char* word()
{
        token = strtok_r(rest, " \t\n", &rest); 
	strupr(token);
        return token;
}


void p_dot_dict() 
{
	dent_s* dw = latest;
	while(dw) {
		printf("echo_dict: location=%p, prev=%p, flags=%d len=%d\n", dw, dw->prev, dw->flags, dw->len);
		printf("word=<");
		for(int i = 0; i< dw->len; ++i) printf("%c", dw->name[i]);		
		//size_t p =  (size_t) code(dw);
		//for(int i = 0; i<8; ++i) { printf("%x",  (unsigned int)(p % 8));  p = p/8; }
				
		//codeptr p1 = **p;
		//memcpy(&p, code(dw), sizeof(void*));
		//p1();
		codeptr p1 = *(codeptr*) code(dw);
		printf(">, code() = %p, %p\n", code(dw), p1);
		//puts("");
		dw = dw->prev;
	}
}
void p_words() {
	dent_s* dw = latest;
	while(dw) {
		for(int i = 0; i < dw->len; ++i) putchar(dw->name[i]);
		puts("");
		dw = dw->prev;
	}
}

void p_dots()
{
	printf("Stack: (%d):", tos);
	for(int i = 0; i< tos; ++i) printf("%ld ", dstack[i]);
}

void p_plus() { push(pop() + pop()); }
void p_minus() { cell_t v1 = pop(), v2 = pop(); push(v2-v1); }
void p_mult() { push(pop() * pop()); }
void p_div() { cell_t v1 = pop(), v2 = pop(); push(v2/v1); }

void p_dot() { printf("%ld ", pop()); }

void p_tick()
{
	char* token = word();
	dent_s* dw = find(token);
	if(dw == NULL)
		undefined(token);
	else {
		push((cell_t)dw);
	}
}

void dotname (dent_s* dw) /// print a word's name given its dictionary address
{
	//dent_s* dw = (dent_s*) pop();
	for(int i = 0; i< dw->len; ++i)
		printf("%c", dw->name[i]);
	printf(" ");
}


void p_dotname() /// print a word's name given its dictionary address
{
	dotname((dent_s*) pop());
}

void p_exit ()
{
	//puts("p_exit called");
	IP = (cell_t*) rpop();
}



void docol ()
{

	//puts("docol TODO NOW tricky!");

	// cache the dictionary location for EXIT
	static dent_s* dw_exit = 0;
	if(!dw_exit) dw_exit = find("EXIT");	

	dent_s* dw;
	IP=wptr;
	IP++; // skip of myself (docol)
	for(;;) {
		dw = (dent_s*) dref(IP++);
		if(dw == dw_exit) break;
		rpush((cell_t) IP);
		xdw(dw);
		IP = (cell_t*) rpop();
	}
}

//void execute(codeptr fn) { fn(); }


void xdw (dent_s* dw)
{
	wptr = code(dw);
	codeptr fn = (codeptr) dref(wptr);
	fn();
}

void p_execute() { xdw((dent_s*) pop()); }

void p_hi() { puts("hello world"); }

void p_colon()
{
	word();
	createz(0, token, docol);
	//create_header(F_SYN, token);
	compiling = true;
}

void p_semi()
{
	heapify_word("EXIT");
	compiling = false;
}

void p_lit()
{
	cell_t v = dref((void*) rstack[rtop-1]);
	rstack[rtop-1] += sizeof(cell_t);
	push(v);
}

void embed_literal(cell_t v)
{
	heapify_word("LIT");
	heapify(v);
}

void p_qbranch()
{
	cell_t offset = 2; 
	if(pop()) offset += dref((void*) rstack[rtop-1]+ sizeof(cell_t));
	rstack[rtop-1] += offset * sizeof(cell_t);
}
void p_branch()
{
	//cell_t offset = dref((void*) rstack[rtop-1]+ sizeof(cell_t))+2;
	//rstack[rtop-1] += offset * sizeof(cell_t);
	rstack[rtop-1] = dref((void*) rstack[rtop-1]); //+ sizeof(cell_t));
}

void p_dup()
{
	cell_t v = pop();
	push(v); 
	push(v);
}
void p_here () { push((cell_t)hptr); }

void p_immediate() { latest->flags |= F_IMM; }

void p_lsb() { compiling = false; }
void p_rsb() { compiling = true; }

//void p_comma() { heapify((void*)pop()); hptr += sizeof(cell_t); }
void p_comma() { heapify(pop()); }
void p_swap() { cell_t temp = dstack[tos-1]; dstack[tos-1] = dstack[tos-2]; dstack[tos-2] = temp; }
void p_at () { push(dref((void*)pop())); }
void p_exc() { cell_t pos = pop(); cell_t val = pop(); store(pos, val); }
void p_allot() { hptr += pop(); }

void _create() { push((cell_t)wptr); }
void p_create() { word(); createz(0, token, _create); }

void p_char() { word(); push(token[0]);}
void p_emit() { printf("%c", (char)pop()); }



typedef struct {byte flags; char* zname; codeptr fn; } prim_s;
prim_s prims[] =  {
	{0,	"CHAR",	p_char},
	{0,	"EMIT", p_emit},
	{0,	"CREATE", p_create},
	{0,	"ALLOT", p_allot},
	{0,	"!",	p_exc},
	{0,	"@",	p_at},
	{0,	"SWAP", p_swap},
	{0,	",", p_comma},
	{F_IMM,	"[", p_lsb},
	{0,	"]", p_rsb},
	{0,	"IMMEDIATE", p_immediate},
	{0,	"HERE", p_here},
	{0,	"DUP", p_dup},
	{0,	"BRANCH", p_branch},
	{0,	"?BRANCH", p_qbranch},
	{0,	"LIT", p_lit},
	{0,	"EXIT", p_exit},
	{0,	".NAME", p_dotname},
	{0,	"HI", p_hi},
	{0,	"'", p_tick},
	{0,	"EXECUTE", p_execute},
	{0, 	":", p_colon},
	{F_IMM,	";", p_semi},
	{0, 	".", p_dot},
	{0, 	"WORDS", p_words},	
	{0, 	".S", p_dots},
	{0, 	"+", p_plus},
	{0, 	"-", p_minus},
	{0, 	"*", p_mult},
	{0, 	"/", p_div},
	{0,	".DICT", p_dot_dict},
	0
};
void add_primitives()
{
	prim_s* p = prims;
	while(p->zname) {
		//p->len = strlen(p->zname);
		//printf("add_prim: strlen=%ld, codeptr= %p\n", strlen(p->zname), p->fn);
		createz(p->flags, p->zname, (codeptr) p->fn);
		p++;
	}
}

char* derived[] = {
	": VARIABLE	create 42 , ;",
	": 1+ 1 + ;",
	0
};

void add_derived()
{
	//char* line = derived[0; 
	char** strs = derived;
	while(*strs) {
		eval_string(*strs++);
	}

}


void eval_string(char* str)
{
	strncpy(tib, str, sizeof(tib));
	process_tib();
}

void process_tib()
{
	rest = tib;
	while(word()) process_word();
}


void process_word()
{
	dent_s* dw = find(token);
	if(dw == 0) {
		cell_t v;
		//str2int_errno res = str2int(&v, token, 10);
		//if(res == STR2INT_SUCCESS) {
		if(int_str(token, &v)) {
			if(compiling)  
				embed_literal(v);
			 else 
				push(v);
		} else { 
			undefined(token);
		}
	} else {
		if(compiling && !(dw->flags & F_IMM)) 
			heapify_dw(dw);
		else 
			xdw(dw);
	}
}


int main()
{
	compiling = false;
	add_primitives();
	add_derived();
	while(fgets(tib, sizeof(tib), stdin)) {
		process_tib();
		puts("  ok");
	}

	return 0;
}
