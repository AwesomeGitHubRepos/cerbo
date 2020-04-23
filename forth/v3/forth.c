/*
 * I release this file into the Public Domain, together with
 * all the other files in this directory.
 *
 * Enjoy!
 *
 * Mark Carter, Jan 2020
 *
 */

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* GNU C preprocessor definitions can be obtained by executing:
 * gcc -dM -E - < /dev/null
 */

//typedef intptr_t cell_t;
#if(__SIZEOF_POINTER__ ==4)
typedef int32_t cell_t;
const char* cell_fmt = "%ld";
//typedef float flt_t; // mdunno if this is true of rnot
#endif
#if(__SIZEOF_POINTER__ ==8)
typedef int64_t cell_t;
const char* cell_fmt = "%ld";
//typedef double flt_t;
#endif

#if(__SIZEOF_POINTER__ == __SIZEOF_FLOAT__)
typedef float flt_t;
#define STR2F strtof
const char* flt_fmt = "%f ";
#endif
#if(__SIZEOF_POINTER__ == __SIZEOF_DOUBLE__)
typedef double flt_t;
#define STR2F strtod
const char* flt_fmt = "%f ";
#endif

#define DEBUG(cmd) cmd
#define DEBUGX(cmd)


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

//bool compiling = false;
cell_t state = 0; // 0=>interpretting, 1=>compiling
bool show_prompt = true;
ubyte flags = 0;

char tib[132];
int bytes_read = 0; // number of bytes read into TIB
enum yytype_e { unk, str, inum, flt}; // inum means integer number
enum yytype_e yytype;
cell_t yylval;


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

bool int_flt(char* s, cell_t* v)
{
	char* endptr;
	flt_t f = STR2F(s, &endptr);
	memcpy(v, &f, sizeof(cell_t));
	//printf("int_flt:last char is <%d>\n", *endptr);
	return *endptr == 0; // there shouldn't be anything left to process if it's a valid float

}

void undefined(char* token)
{
	printf("undefined word:<%s>\n", token);
}

cell_t dref (void* addr) { return *(cell_t*)addr; }

void store (cell_t pos, cell_t val) { *(cell_t*)pos = val; }

void heapify (cell_t v)
{
	store((cell_t)hptr, v);
	hptr += sizeof(cell_t);
}

void create_header (char* zname)
{
	char* name = hptr;
	ubyte noff = 0; // name offset
	for(noff = 0 ; noff<= strlen(zname); ++noff) *hptr++ = toupper(zname[noff]); // include trailing 0
	dent_s dw;
	dw.prev = latest;
	dw.flags = noff;
	memcpy(hptr, &dw, sizeof(dw));
	latest = (dent_s*) hptr;
	hptr += sizeof(dw);
}

void createz (char* zname, cell_t acf) // zname being a null-terminated string
{
	create_header(zname);
	heapify(acf);
}

char* name_dw(dent_s* dw)
{
	char* str = (char*) dw;
	int name_off =  dw->flags & 0b111111;
	str -= name_off; 
	return str;
}

void* code(dent_s* dw)
{
	return ++dw;
}

codeptr cfa_find (char* name) // name can be lowercase, if you like
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

void str_begin (cell_t* loc)
{
	*loc = (cell_t)hptr + 0* sizeof(cell_t);
	if(!state) return;
	heapify_word("BRANCH");
	*loc = (cell_t) hptr;
	heapify(2572); // leet for zstr
}

void str_end (cell_t loc)
{
	if(state) {
		store(loc, (cell_t)hptr); // backfil to after the embedded string
		heapify_word("LIT");
		heapify(loc + sizeof(cell_t));
	} else
		push(loc); 
}

void p_z_slash () 
{ 
	cell_t loc;
	str_begin(&loc);
 
	token += 3; // movwe beyong the z" 
	while(1) {
		*hptr = *token;
		if(*hptr == '"' || *hptr == 0) break;
		hptr++;
		token++;
	}
	rest = token;
	*hptr = 0;
	hptr++;

	str_end(loc);
}

void identify_word()
{
	yytype = unk;
	if(*rest == '"') {
		//puts("parse_world: parsing string");
		yytype = str;
		token++;
		rest++;
		while(*rest != '"' && *rest)  rest++; 
		return;
	}

	while(!isspace(*rest) && *rest) rest++;
	*rest = 0;

	cell_t v;
	if(int_str(token, &v)) {
		yytype = inum;
		yylval = v;
		return;
	}

	if(int_flt(token, &v)) {
		yytype = flt;
		//yylval.f = v;
		yylval = v;
		return;
	}
}

char* parse_word () 
{ 
	if(rest == 0) {
		token = tib;
	} else {
		token = rest + 1;
	}

	if(*token ==0) return 0;
	while(isspace(*token)) token++;
	rest = token;
	identify_word();

	*rest = 0;
	if(*token == 0) token = 0;
	return token;
}

void p_parse_word ()
{
	push((cell_t)parse_word());
}

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
	for(int i = 0; i< sstack.size; ++i) {
		printf(cell_fmt, sstack.contents[i]);
		printf(" ");
	}
}


void p_plus() { push(pop() + pop()); }
void p_minus() { cell_t v1 = pop(), v2 = pop(); push(v2-v1); }
void p_mult() { push(pop() * pop()); }
void p_div() { cell_t v1 = pop(), v2 = pop(); push(v2/v1); }

void p_pcd() { printf(cell_fmt,  pop()); }
void p_dot() { p_pcd(); printf(" "); }


void execute (codeptr cfa)	
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
	static codeptr cfa_exit = 0, cfa_semi = 0;
	if(cfa_exit==0) cfa_exit = cfa_find("EXIT");
	if(cfa_semi==0) cfa_semi = cfa_find(";");
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
	state = false;
}

void p_colon()
{
	parse_word();
	createz(token, (cell_t) docol); 
	state = true;
}


void p_at () { push(dref((void*)pop())); }
void p_exc() { cell_t pos = pop(); cell_t val = pop(); store(pos, val); }

void _create () 	{ push((cell_t)++W); }
void p_create() 	{ parse_word(); createz(token, (cell_t) _create); }
void p_dlr_create () 	{ createz((char*) pop(), (cell_t) _create); }
void p_comma ()		{ heapify(pop()); }
void p_prompt ()	{ show_prompt = (bool) pop(); }

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


void p_type () { printf("%s", (char*) pop()); }

void p_dot_slash () 
{
	p_z_slash();
	if(state)
		heapify_word("TYPE");
	else
		p_type();
}

void p_emit() { printf("%c", (char)pop()); }
void p_swap() { cell_t temp = STOP; STOP = STOP_1; STOP_1 = temp; }
void p_immediate() { latest->flags |= F_IMM; }

bool is_immediate(codeptr cfa) 
{
	dent_s* dw = (dent_s*) cfa;
	dw--;
	return (dw->flags & F_IMM);
}

void p_0branch()
{
	if(!pop()) 
		RTOP = dref((void*) RTOP);
	else
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


void p_bslash () 
{ 
	while(*rest) rest++;
	rest--;
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
	parse_word();
	createz(token, (cell_t) docol);

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
	parse_word();
	DEBUGX(printf("defer:token:%s\n", token));
	createz(token, (cell_t) docol);
	heapify_word("XDEFER");
	heapify_word("EXIT");
	heapify_word(";");
}



void p_is ()
{
	parse_word();
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

bool has_embedded_lit(char* name)
{
	static char *lits[] = {"LIT", "0BRANCH", "?BRANCH", "BRANCH"};
	char **str = lits;
	do {
		if(streq(name, *str)) return true;
	} while(*++str);
	return false;
}

void p_see()
{
	static cellptr cfa_docol = 0;
	if(cfa_docol == 0) cfa_docol = (cellptr) dref(cfa_find("DOCOL"));
	parse_word();
	cellptr cfa = (cellptr) cfa_find(token);
	if(cfa == 0) { puts("UNFOUND"); return; }
	printf(": %s\n",token);
	
	if(is_immediate((codeptr) cfa)) puts("IMMEDIATE");	
	
	if((cellptr) dref(cfa) != cfa_docol) {
		puts("PRIM");
		return;
	} // so far, this works


	while(1) {
		cellptr cfa1 = (cellptr) dref(++cfa);
		char* name = name_cfa(cfa1);
		puts(name);
		if(has_embedded_lit(name)) printf("%ld\n", *(++cfa));
		if(streq(name, ";")) break;
	}


}


void p_cell () { push(sizeof(cell_t)); }
void p_gt () { push(pop() <  pop()); }
void p_ge () { push(pop() <= pop()); }
void p_lt () { push(pop() >  pop()); }
void p_le () { push(pop() >= pop()); }
void p_eq () { push(pop() == pop()); }
void p_ne () { push(pop() != pop()); }

void p_pick ()
{
	int n = pop();
	n = sstack.size - n -1;
	if(n>=0)
		push(sstack.contents[n]);
	else
		puts("Stack underflow");
}

bool refill ()
{ 
	rest = 0; 
	memset(tib, 0, sizeof(tib));
	//tib[0] = 0; 
	return fgets(tib, sizeof(tib), stdin); 
}

void p_refill () { push(refill()); }
void p_tib ()	{ push((cell_t) tib); }

void p_str_eq () { push(strcmp((const char*) pop(), (const char*) pop()) == 0); }
void p_str_lt () { push(strcmp((const char*) pop(), (const char*) pop()) < 0); }
void p_strn_eq () { int n = pop(); push(strncmp((const char*) pop(), (const char*) pop(), n) == 0); }

void p_int_str ()
{
	cell_t v;
	bool ok = int_str((char*) pop(), &v);
	push(v);
	push(ok);
}

void fpop(flt_t *f)
{
	cell_t f1 = pop();
	memcpy(f, &f1, sizeof(cell_t));
}

void fpush(flt_t f)
{
	cell_t f1;
	memcpy(&f1, &f, sizeof(cell_t));
	push(f1);
}

void p_fdot()
{
	flt_t f;
	fpop(&f);
	printf(flt_fmt, f);
}

void p_ftimes()
{
	flt_t f1, f2;
	fpop(&f1);
	fpop(&f2);
	fpush(f1*f2);
}

void p__char_()
{
	parse_word();
	embed_literal(*token);
}

void p_pt();

void p_round()
{
	flt_t f;
	fpop(&f);
	f = roundf(f);
	push((cell_t)f);
}

void p_pcnd ()
{
	char fmt[10];
	sprintf(fmt, "%%%dd", (int) pop());
	printf(fmt, pop());
}

void p_pc0nd ()
{
	char fmt[10];
	sprintf(fmt, "%%0%dd", (int) pop());
	printf(fmt, pop());
}

void p_fneg()
{
	flt_t f;
	fpop(&f);
	fpush(-f);
}



void p_floor() { flt_t f; fpop(&f); int i = (int) floor(f); push(i); }
void p_ceil()  { flt_t f; fpop(&f); int i = (int) ceil(f); push(i); }

void p_noname ()
{
	push((cell_t)hptr);
	//heapify_word("DOCOL");
	heapify((cell_t)docol);
	state = true;
}

void p_find()
{
	push((cell_t)cfa_find((char*)pop()));
}


void p_immediateq ()
{
	cell_t imm  = is_immediate((codeptr) pop());
	push(imm);
}


void p_state() { push((cell_t) &state); }

typedef struct {ubyte flags; char* zname; codeptr fn; } prim_s;
prim_s prims[] =  {
	{0,	"STATE", p_state},
	{0,	"IMMEDIATE?", p_immediateq},
	{0,	"FIND", p_find},
	{0,	":NONAME", p_noname},
	{0,	"%ND", p_pcnd},
	{0,	"CEIL", p_ceil},
	{0,	"FLOOR", p_floor},
	{0,	"%D", p_pcd},
	{0,	"FNEG", p_fneg},
	{0,	"$CREATE", p_dlr_create},
	{0,	"%0ND", p_pc0nd},
	{0,	"ROUND", p_round},
	{0,	"PT", p_pt},
	{F_IMM,	"[CHAR]", p__char_},
	{0,	"F*", p_ftimes},
	{0,	"F.", p_fdot},
	{0,	"STR>INT", p_int_str},
	{0,	"STRN=", p_strn_eq},
	{0,	"STR<", p_str_lt},
	{0,	"STR=", p_str_eq},
	{0,	"TIB", p_tib},
	{0,	"REFILL", p_refill},
	{0,	"PICK", p_pick},
	{0,	"!=", p_ne},
	{0,	"=", p_eq},
	{0,	"<=", p_le},
	{0,	"<", p_lt},
	{0,	">=", p_ge},
	{0,	">", p_gt},
	{0,	"CELL", p_cell},
	{0,	"PARSE-WORD", p_parse_word},
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
	{F_IMM,	"\\", p_bslash},
	{0,	">R", p_tor},
	{0,	"R>", p_fromr},
	{0, 	"TYPE", p_type},
	{0, 	"0BRANCH", p_0branch},
	{0, 	"IMMEDIATE", p_immediate},
	{0, 	"SWAP", p_swap},
	{0, 	"EMIT", p_emit},
	{F_IMM,	"Z\"", p_z_slash},
	{F_IMM,	".\"", p_dot_slash},
	{0,	 "DUP", p_dup},
	{0, 	"?BRANCH", p_qbranch},
	{0, 	"HERE", p_here},
	{0, 	"PROMPT", p_prompt},
	{0, 	",", p_comma},
	{0, 	"CREATE", p_create},
	{0, 	"!", p_exc},
	{0, 	"@", p_at},
	{0,	":", p_colon},
	{F_IMM,	";", p_semi},
	{0,	"EXECUTE", p_execute},
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
		createz(p->zname, (cell_t) p->fn);
		if(p->flags) p_immediate();
		p++;
	}
}

void eval_string(char* str)
{
	strncpy(tib, str, sizeof(tib));
	process_tib();
}

char* derived[] = {
	": EXIT		;",
	": CELL+	cell + ;",
	": COMPILE	r> dup @ , cell+ >r ;",
	": BRANCH	r> @ >r ;",
	": IF 		compile 0branch here 0 , ; immediate",
	": THEN 	here swap ! ; immediate",
	": ELSE 	compile branch here >r 0 , here swap ! r> ; immediate", 
	": POSTPONE	parse-word find dup immediate? if else compile compile then , ; immediate",
	": ` 		postpone postpone ; immediate", // simple alias
	": LITERAL	postpone lit  ,   ; immediate",
	": VARIABLE 	create 0 , ;",
	": 1+ 		1 + ;",
	": TAB		9 emit ;",
	": CR 		10 emit ;",
	": CONSTANT 	<builds , does> @ ;",
	": BEGIN 	here ; immediate",
	": ?AGAIN 	postpone ?branch , ; immediate",
	": WHILE	postpone if ; immediate",
	": REPEAT	1 postpone literal postpone else 0 postpone literal postpone then  postpone ?again ; immediate",
	": CELLS	cell * ;",
	": CELLS+	cells + ;",
	": OVER		1 pick ;",
	": SPACE	32 emit ;",	
	": ++		dup @ 1+ swap ! ;",
	": NOT		0 = if 1 else 0 then ;",
	": UNDEFINED	\"Undefined word:<\" type type \">\" type cr ;",
	": .UNDEFINED	\"ERR: undefined word\" type cr ;",
	": [		0 state ! ; immediate",
	": ]		1 state ! ;",
	": ' 		parse-word dup find dup if swap drop else  drop undefined  [ parse-word .undefined find ] literal then ;",
	": [']		' postpone literal ; immediate",
	": LINE( 	` begin ` parse-word ` dup ` while ; immediate",
	": )LINE 	` repeat ` drop ; immediate",
	": VARS:        line( $create 0 , )line ;",	
	": EXPECT	 cr \"Expect \" type type  \":\" type cr ;",
	": NEG		0 swap - ;",

	0
};

void add_derived()
{
	char** strs = derived;
	while(*strs) {
		eval_string(*strs++);
	}

}

void process_token (char* token)
{
	if(yytype == str) {
		cell_t loc;
		str_begin(&loc);
		while(token <= rest) { 
			*hptr++ = *token++;
			//if(token == rest) break;
		}
		str_end(loc);
		return;
	}

	if(yytype == inum || yytype == flt) {
		cell_t v = yylval;

		if(state)
			embed_literal(v);
		else
			push(v);
		return;
	}

	codeptr cfa = cfa_find(token);

	if(cfa == 0) {
		undefined(token);
		return;
	}


	if(state && !(flags & F_IMM))
		heapify((cell_t)cfa);
	else
		execute(cfa);
}

void p_pt()
{
	process_token((char*) pop());
}

void process_tib()
{
	rest = 0;
	while(parse_word()) process_token(token);
}
int main()
{
	assert(sizeof(size_t) == sizeof(cell_t));
	assert(sizeof(flt_t) == sizeof(cell_t));
	//printf("sizeof double=%ld, float=%ld, cell=%ld\n", sizeof(double), sizeof(float), sizeof(cell_t));
	state = false;
	add_primitives();
	add_derived();


	while(refill()) {
		process_tib();
		if(show_prompt) puts("  ok");
	}
	return 0;
}
