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
#include <setjmp.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
//#include <stdio.h>
#include <string.h>

#include "forth.h"


jmp_buf env_buffer;

//typedef intptr_t cell_t;
#if(__SIZEOF_POINTER__ ==4)
typedef int32_t cell_t;
const char* cell_fmt = "%ld ";
#endif
#if(__SIZEOF_POINTER__ ==8)
typedef int64_t cell_t;
const char* cell_fmt = "%ld ";
#endif

#define DEBUG(cmd) cmd
#define DEBUGX(cmd)

#define TODO(x) puts(x)

void ACCEPT();
void BL();
void FIND();
void QUIT();
void WORD();
void p_create();
void mk_docol();
void to_CFA();

typedef cell_t* cellptr;
typedef uint8_t ubyte;
typedef void (*codeptr)();

// some common cf's, which we fill in during initialisation
static codeptr cfa_exit = 0, cfa_semi = 0, cfa_docol =0;

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

static char word_pad[64]; // scratch counted buffer to hold word found
char tib[132];
//int bytes_read = 0; // number of bytes read into TIB
int ntib=0; // #TIB
int in=0; // >IN

typedef struct dent { // dictionary entry
	// name of word is packed before this struct
	struct dent* prev;
	ubyte  flags;
} dent_s;

dent_s *latest = NULL; // latest word being defined

//const ubyte F_IMM = 1 << 7;
#define F_IMM (1<<7)

int isspace(int c)
{
	return ((c=='\t') || (c==' ') || (c=='\r') || (c=='\n'));
}

char toupper(char c)
{
	if(('a'<=c) && (c <= 'z'))
		return c+ 'A' - 'a';
	else
		return c;
}

int strcasecmp(const char *s1, const char *s2)
{
	while(*s1 && *s2) {
		if(toupper(*s1) != toupper(*s2)) break;
		s1++;
		s2++;
	}
	return toupper(*s1) - toupper(*s2);
}


/* not sure if this is strictly necessary
 * because we use strcasecmp instead of strcmp
 */
char* strupr(char* str) 
{ 
	int c = -1, i =0;
	if(!str) return NULL;
	while((c = toupper(str[i]))) {
		str[i] = c;
		i++;
		if(c==0) return str;
	}
	return str;
}

bool int_str(const char*s, cell_t *v)
{
	*v = 0;
	if((s==0) || (*s==0)) return false;
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

void undefined(const char* token){
	printf("undefined word:<%s>\n", token);
}

cell_t dref (void* addr) { return *(cell_t*)addr; }
cell_t dref (cell_t addr) { return *(cell_t*)addr; }

void store (cell_t pos, cell_t val) { *(cell_t*)pos = val; }

void heapify (cell_t v)
{
	store((cell_t)hptr, v);
	hptr += sizeof(cell_t);
}


void print_cstr(char* cstr)
{
	ubyte len = *cstr++;
	while(len--) putchar(*cstr++);
}

void debug_cstr(char* cstr)
{
	printf("debug_str:%d:<", *cstr);
	print_cstr(cstr);
	puts(">");
}



void create_full_header(ubyte flags, const char* cstr, codeptr fn)
{
	ubyte noff = 0; // name offset
	ubyte len = *cstr;
	memcpy(hptr, cstr, len+1);
	hptr += len +1;
	dent_s dw;
	dw.prev = latest;
	dw.flags = flags | len;
	memcpy(hptr, &dw, sizeof(dw));
	latest = (dent_s*) hptr;
	hptr += sizeof(dw);
	heapify((cell_t) fn);
}


void create_header(const char* cstr, codeptr fn)
{
	create_full_header(0, cstr, fn);
}

char* name_dw (dent_s* dw)
{
	char* str = (char*) dw;
	int name_off =  dw->flags & 0b111111;
	str = str-name_off-1; 
	return str;

}

void* code(dent_s* dw)
{
	return ++dw;
}

codeptr cfa_find(const char* name) // name can be lowercase, if you like
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

void heapify_word(const char* name)
{
	codeptr xt = cfa_find(name);
	heapify((cell_t) xt);
}

void embed_literal(cell_t v)
{
	heapify_word("LIT");
	heapify(v);
}


//char* token;
//char* rest;

#if 0
void  get_word () 
{ 
	BL(); WORD();

	/*
	  if(rest == 0) {
		token = tib;
	} else {
		token = rest + 1;
	}

	if(*token ==0) return 0;
	while(isspace(*token)) token++;
	rest = token;
	while(!isspace(*rest) && *rest) rest++;
	*rest = 0;
	strupr(token);
	//printf("word:toke:<%s>\n", token);
	if(*token == 0) token = 0;
	return token;
	*/
}
#endif


//void process_tib();


void p_hi() { puts("hello world"); }

void p_words() {
	dent_s* dw = latest;
	while(dw) {
		char* cstr = name_dw(dw);
		print_cstr(cstr);		
		puts("");
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
	for(int i = 0; i< sstack.size; ++i) printf(cell_fmt, sstack.contents[i]);
}


void p_plus() { push(pop() + pop()); }
void p_minus() { cell_t v1 = pop(), v2 = pop(); push(v2-v1); }
void p_mult() { push(pop() * pop()); }
void p_div() { cell_t v1 = pop(), v2 = pop(); push(v2/v1); }

void p_dot() { printf(cell_fmt, pop()); }

void p_tick()
{
	puts("TODO p_tick");
#if 0
	get_word();
	codeptr cfa = cfa_find(token);
	if(cfa)
		push((cell_t) cfa);
	else
		undefined(token);
#endif
}

void execute (codeptr cfa)	
{
	W =  (cellptr) cfa;
	codeptr fn = (codeptr) dref((void*) cfa);
	fn();
}
void p_execute()
{
	execute((codeptr)pop());
}

char* name_cfa(cellptr cfa)
{
	dent_s* dw = (dent_s*) cfa;
	return name_dw(--dw);
}

void docol()
{
	codeptr cfa;
	cellptr IP = W;
	IP++;
	for(;;) {
		cfa = (codeptr) dref(IP++);
		if(cfa == cfa_exit || cfa == cfa_semi) break;
		rpush((cell_t)IP);
		execute(cfa);
		IP = (cellptr) rpop();
	}

}

void p_semi()
{
	heapify_word("EXIT");
	heapify_word(";"); // added for the convenience of SEE so as not to confuse it with EXIT
	compiling = false;
}

void p_colon()
{
	mk_docol(); 
	compiling = true;
}

void p_exit()
{ 
	// TODO: is this right? I'm not sure this function is ever called
	rpop();
}

void p_at () { push(dref((void*)pop())); }
void p_exc() { cell_t pos = pop(); cell_t val = pop(); store(pos, val); }

void _create() { push((cell_t)++W); }

void get_create(codeptr fn)
{
	BL();
	WORD();
	create_header(word_pad, fn);
}
void p_create() { get_create(_create); }
void mk_docol() { get_create(docol); }

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

void p_z_slash () 
{ 
	puts("TODO p_z_slash");
#if 0
	//cell_t loc = (cell_t)hptr + sizeof(cell_t);
	cell_t loc = (cell_t)hptr + 0* sizeof(cell_t);
	if(compiling) {
		heapify_word("EMBIN");
		loc = (cell_t) hptr;
		heapify(2572); // leet for zstr
	}

	//char* src = 0; // delim_word("\"", false);

	//do {} while(*hptr++ = *src++);
	token += 3; // move beyong the z" 
	while(1) {
		*hptr = *token;
		if(*hptr == '"' || *hptr == 0) break;
		hptr++;
		token++;
	}
	rest = token;
	*hptr = 0;
	hptr++;

	// alignment issues?
	//while((cell_t) hptr % sizeof(cell_t)) hptr++;

	if(compiling) {
		store(loc, (cell_t)hptr); // backfil to after the embedded string
		heapify_word("LIT");
		heapify(loc + sizeof(cell_t));

	} else
		push(loc); 
#endif
}

void p_type () { printf("%s", (char*) pop()); }

void p_dot_slash () 
{
	p_z_slash();
	if(compiling)
		heapify_word("TYPE");
	else
		p_type();
}

void p_emit() { printf("%c", (char)pop()); }
void p_swap() { cell_t temp = STOP; STOP = STOP_1; STOP_1 = temp; }
void p_immediate() { latest->flags |= F_IMM; }

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

void p_bslash () 
{
	puts("TODO p_baslah");
}

void p_drop () { pop(); }


void p_dodoes() // not an immediate word
{
	DEBUGX(puts("calling dodoes"););
	cell_t does_loc  = pop(); // provided by 555. Previous cell should be an EXIT

	cell_t loc888 = (cell_t) code(latest) + 4*sizeof(cell_t);
	DEBUGX(printf("dodoes thinks 888 is located at %p\n", (void*) loc888););
	store(loc888, does_loc);

	cell_t loc777 = loc888 - 2*sizeof(cell_t);
	DEBUGX(printf("dodoes thinks 777 is located at %p\n", (void*) loc777););
	cell_t offset = loc888 + 2*sizeof(cell_t); // points to just after the ';' of the docol 
	store(loc777, offset); 
}


void p_does() // is immeditate
{
	heapify_word("LIT");
	cell_t loc = (cell_t) hptr; // loc holds the location of 555
	heapify(555); // ready for backfill
	heapify_word("(DOES>)"); // aka p_dodoes
	heapify_word("EXIT");
	store(loc, (cell_t)hptr); // now backfill it
	// so now 555 has been replaced by the cell after the EXIT, which gets pushed onto the stack
	// so that (DOES>) knows where it is on the heap


}


void p_builds () // not an immediate word
{
	mk_docol();

	heapify_word("LIT");
	DEBUGX(printf("p_builds: location of 777: %p\n", hptr););
	heapify(777); // filled in properly by dodoes

	heapify_word("BRANCH");
	DEBUGX(printf("p_builds: location of 888: %p\n", hptr););
	heapify(888);

	heapify_word(";");
}

void p_xdefer()
{
	puts("DEFER not set");
}

void p_defer()
{
	//puts("defer:called");
	//get_word();
	//DEBUGX(printf("defer:token:%s\n", token));
	mk_docol();
	heapify_word("XDEFER");
	heapify_word("EXIT");
	heapify_word(";");
}



void p_is ()
{
	puts("TODO p_is");
#if 0
	get_word();
	cell_t cfa = (cell_t) cfa_find(token);
	if(cfa) {
		cell_t offset = cfa + 1 *sizeof(cell_t);
		DEBUGX(printf("IS:offset: %p\n", (void*) offset));
		DEBUGX(printf("IS:xdefer: %p\n", p_xdefer));
		cellptr xt = (cellptr) pop(); 
		DEBUGX(printf("IS:token name:%s", name_cfa((cellptr)xt)));
		store(offset, (cell_t)xt);
		return;
	} else
		undefined(token);
#endif
}

void p_len()
{
	push(strlen((const char*) pop()));
}

void p_name()
{
	puts(name_cfa((cellptr) pop()));
}

bool streq(const char* str1, const char* str2)
{
	return strcmp(str1, str2) == 0;
}

void p_see()
{
	puts("TODO see");
	
	BL(); WORD(); FIND(); to_CFA();
	//get_word();
	//codeptr cfa = (codeptr) pop();
	cell_t loc = pop();
	if(loc == 0) { puts("UNFOUND"); return; }

	// determine if immediate
	dent_s* dw = (dent_s*) loc;
	dw--;
	if(dw->flags & F_IMM) puts("IMMEDIATE");

	if(dref(loc) != (cell_t) cfa_docol) {
		puts("PRIM");
		return;
	} 


	while(1) {
		cellptr cfa = (cellptr) dref(++loc);
		//cfa += sizeof(cell_t);
		//loc++;
		printf(".");
		if(cfa == (cellptr) cfa_semi) return;
#if 0
		char* name = name_cfa(cfa1);
		puts(name);
		if(streq(name, "LIT")) printf("%ld\n", *(++cfa));
		if(streq(name, "EMBIN")) {
			cellptr fin = (cellptr) dref(++cfa);
			char* cptr = (char*) (++cfa);
			while(cptr != (char*) fin) {				
				printf("%c", *cptr);
				cptr++;
			}
			puts("\nEMBIN END");
			cfa = --fin;
		}
		if(streq(name, ";")) break;
#endif
	}
}


void BL() { push(32); } // a space

bool match_name(char* cstr, dent_s* dw)
{
	if(cstr == 0 || dw == 0) return false;
	//int dw_len = dw->flags & 0b01111111;
	ubyte len = *cstr;
	char* dw_cstr = name_dw(dw);
	if(len != *dw_cstr) return false;
	while(len--) if(toupper(*++cstr) != toupper(*++dw_cstr)) return false;
	/*
	for(int i = 0; i< cstr_len; i++) {
		if(toupper(*(cstr+i+1)) != toupper(*(dw_str+i))) return false;
	}
	*/
	return true;

}

/* returns the dictionary header, if the word is found. I'm being consistent with Jonesforth
*/

void FIND()
{
	char* cstr = (char*) pop();
	dent_s* dw = latest;
	while(dw) {
		if(match_name(cstr, dw)) {
			//puts("FIND:matched");
			flags = dw->flags;
			//if(flags & F_IMM) puts("FIND: it's an immediate word");
			push((cell_t)dw);
			return;
		}

		dw = dw->prev;
		//print_cstr(name_dw(dw));
	}
	push(0);
}

void WORD()
{
	char delim = pop();
	//int size=0;
	unsigned char len = 0;
	while(tib[in] == delim) in++; // skip leading delimiters
	while(in < ntib) {
		char c = tib[in];
		if(c == delim) break;
		len++;
		word_pad[len] = c;
		in++;
	}
	word_pad[0] = len;
	push((cell_t)word_pad);
}

void to_CFA()
{
	dent_s* dw = (dent_s*) pop();
	if(dw==0) {
		push(0);
		return;
	}
	push((cell_t) ++dw);
}

void _abort(const char* zstr)
{
	puts(zstr);
	longjmp(env_buffer, 0);

}

void NUMBER()
{
	char* cstr = (char*) pop();
	int len = *cstr++;
	int sgn = 1;
	int num = 0;

	if(len==0) goto fail;

	if(*cstr == '+' || *cstr == '-') {
		if(len==1) goto fail;
		if(*cstr=='-') sgn = -1;
		cstr++;
		len--;
	}

	for(int i = 0; i< len; i++) {
		char c = *cstr++;
		if(c < '0' || c > '9') goto fail;
		num = num* 10 + c-'0';
	}

	num *= sgn;
	push(num);
	return;
fail:
	_abort("Unrecognised");
}

void interpret_unfound()
{
	push((cell_t)word_pad);
	NUMBER();
	cell_t num = pop();
	if(compiling)
		embed_literal(num);
	else
		push(num);

}
void interpret_found(cell_t cfa)
{
	//codeptr xt = (codeptr) dref((void*)cfa);
	if(compiling && !(flags & F_IMM)) 
		heapify(cfa);
	else
		execute((codeptr)cfa);

	//fn();
	//puts("TODO INTERPRET");
}

/* If the word is found, it will be either executed (if it is an IMMEDIATE word, or if in the "interpret" state, STATE=0) or compiled into the dictionary (if in the "compile" state, STATE<>0). If not found, Forth attempts to convert the string as a number. If successful, LITERAL will either place it on the parameter stack (if in "interpret" state) or compile it as an in-line literal value (if in "compile" state). If not a Forth word and not a valid number, the string is typed, an error message is displayed, and the interpreter ABORTs. This process is repeated, string by string, until the end of the input line is reached. 
 * */
void INTERPRET()
{
	while(1) {
		BL();
		WORD();
		char* word = (char*) pop();
		if(word[0] == 0) break;
		push((cell_t)word);
		FIND();
		to_CFA();

		cell_t cfa = pop();
		if(cfa) 
			interpret_found(cfa);
		else 
			interpret_unfound();

	}
	//p_dots(); seems to check out


}

/* See also QUERY
 * http://www.mosaic-industries.com/embedded-systems/legacy-products/qed2-68hc11-microcontroller/software/chapter_16_advanced_topics
 * */
void QUERY()
{
	ntib = 0;
	while(ntib < sizeof(tib)) {
		int c = getchar();
		if((c<=0) || (c=='\n') || (c=='\r')) break;
		tib[ntib++] = c;
		//if(c == '\n') break;
	}
	in = 0; // offset to current position in TIB
}

void ABORT()
{
	puts("ABORT starting");
	rstack.size = 0;
	sstack.size = 0;
	compiling = false; // enter execution mode
	QUIT(); // normally loops forever, but can be ABORTed
}

typedef struct {ubyte flags; const char* zname; codeptr fn; } prim_s;
prim_s prims[] =  {
	{0,	"NUMBER",	NUMBER},
	{0,	"ABORT",	ABORT},
	{0,	">CFA",		to_CFA},
	{0,	"BL", 		BL},
	{0,	"WORD", 	WORD},
	{0,	"FIND", 	FIND},
	{0,	"INTERPRET", 	INTERPRET},
	{0,	"QUERY", 	QUERY},
	{0,	"QUIT", 	QUIT},
	{0,	"EMBIN", p_branch},
	{0,	"DOCOL", docol},
	{0,	"SEE", p_see},
	{0,	".NAME", p_name},
	{0,	"LEN", p_len},
	{0,	"XDEFER", p_xdefer},
	{0,	"DEFER", p_defer},
	{0,	"IS", p_is}, // probably needs to be an immediate word
	{0, 	"<BUILDS", p_builds},
	{F_IMM,	"DOES>", p_does},
	{0, 	"(DOES>)", p_dodoes},
	{0, 	"DROP", p_drop},
	{0, 	"\\", p_bslash},
	{0, 	"BRANCH", p_branch},
	{0,	">R", p_tor},
	{0,	"R>", p_fromr},
	{0, 	"TYPE", p_type},
	{0, 	"COMPILE", p_compile},
	{0, 	"0BRANCH", p_0branch},
	{0, 	"IMMEDIATE", p_immediate},
	{0, 	"SWAP", p_swap},
	{0, 	"EMIT", p_emit},
	{F_IMM,	"Z\"", p_z_slash},
	{F_IMM,	".\"", p_dot_slash},
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

#if 0
void createz (ubyte flags, const char* zname, cell_t acf) // zname being a null-terminated string
{
	create_header(flags, zname);
	heapify(acf);
}
#endif

#if 0
char* cstr_zstr(char* zstr)
{
	if(zstr==0) return 0;
	unsigned char len = strlen(zstr);
	for(unsigned char  i = len; i>0; i++)
		zstr[i] = zstr[i-1];
	zstr[0] = len;
}
#endif

void add_primitives()
{
	prim_s* p = prims;
	//char *zname;
	while((p->zname)) {
		// convert a 0-terminated string to counted string
		ubyte len = strlen(p->zname);
		memcpy(word_pad+1, p->zname, len);
		*word_pad = len;
		//debug_cstr(word_pad);

		create_full_header(p->flags, word_pad, p->fn);
		p++;
	}
}

#if 0
void eval_string(const char* str)
{
	strncpy(tib, str, sizeof(tib));
	process_tib();
}
#endif

const char* derived[] = {
	": VARIABLE create 0 , ;",
	": 1+ 1 + ;",
	": CR 10 emit ;",
	": IF compile 0branch here 0 , ; immediate",
	": THEN here swap ! ; immediate",
	": ELSE compile branch here >r 0 , here swap ! r> ; immediate", 
	": CONSTANT <builds , does> @ ;",
	": BEGIN here ; immediate",
	": ?AGAIN compile ?branch , ; immediate",
	0
};

void add_derived()
{
	puts("TODO add_derived");
#if 0
	const char** strs = derived;
	while(*strs) {
		puts(*strs);
		eval_string(*strs++);
	}
#endif
}

#if 0
void process_token(const char* token)
{
	codeptr cfa = cfa_find(token);
	if(cfa == 0) {
		cell_t v;
		if(int_str(token, &v)) {
			if(compiling) {
				//puts("about to embed literal");
				embed_literal(v);
				//puts("done embedding literal"); // TODO the existence of this line prevents crashing
			} else {
				push(v);
			}
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
	rest = 0;
	while(get_word()) process_token(token);
}

int get_tib() 
{ 
	fgets(tib, sizeof(tib),tib_in); 
	return !feof(tib_in);
}
#endif


/* Attempted implementation of
 * https://www.forth.com/starting-forth/9-forth-execution/
 */

void QUIT()
{
	while(1) {
		rstack.size = 0; // clear return stack
		QUERY();
		INTERPRET();
		printf(" ok \n"); // equiv of ." ok " CR
	}
}


int main_routine()
{
	assert(sizeof(size_t) == sizeof(cell_t));
	compiling = false;
	add_primitives();
	cfa_docol = cfa_find("DOCOL");
	cfa_exit = cfa_find("EXIT");
	cfa_semi = cfa_find(";");

	//puts("added primitives");
#if 0
	add_derived();
#else
	puts("skipped derived");
#endif

	if(0) {
		puts("words are");
		p_words();
		//process_token("words");
		puts("fin");
	}

//	int val; 
	setjmp(env_buffer);
	ABORT();

	/*
	   while(get_tib()) {
	   process_tib();
	   if(show_prompt) puts("  ok");
	   }
	   */

	return 0;
}

