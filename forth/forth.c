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

typedef size_t cell_t;
//static_assert(sizeof(cell_t) == sizeof(char*));
typedef uint8_t byte;
typedef void (*codeptr)();

size_t IP;	// Interpreter Pointer. 

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
	puts("pop() called");
	return dstack[--tos];
}
//#define PUSH(x) *PSP++ = (cell_t) x // push an item onto the parameter stack
//#define POP()   *--PSP //pop an item off the parameter stack

size_t RSP[10]; // Return Stack Pointer, aka RP
uint8_t heap[10000];
uint8_t* hptr = heap;	// User Pointer. Holds the base address of the task's user area
size_t W;	// Working register.  Multi-purpose
size_t X;	// Working register


char _TIB[136]; // The input buffer
char* TIB = _TIB;
int bytes_read = 0; // number of bytes read into TIB


typedef struct { // dictionary entry
	void* prev;
	byte  flags;
	byte  len; // it is OR'd with flags
	char  name[]; // the name will actually be longer
} dent_s;

//dent_s *dict = NULL;
dent_s *latest = NULL; // latest word being defined

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

// from https://stackoverflow.com/questions/7021725/how-to-convert-a-string-to-integer-in-c#7021750
/* Convert string s to int out.
 *
 * @param[out] out The converted int. Cannot be NULL.
 *
 * @param[in] s Input string to be converted.
 *
 *     The format is the same as strtol,
 *     except that the following are inconvertible:
 *
 *     - empty string
 *     - leading whitespace
 *     - any trailing characters that are not part of the number
 *
 *     Cannot be NULL.
 *
 * @param[in] base Base to interpret string in. Same range as strtol (2 to 36).
 *
 * @return Indicates if the operation succeeded, or why it failed.
 */
str2int_errno str2int(int *out, char *s, int base) {
    char *end;
    if (s[0] == '\0' || isspace(s[0]))
        return STR2INT_INCONVERTIBLE;
    errno = 0;
    long l = strtol(s, &end, base);
    /* Both checks are needed because INT_MAX == LONG_MAX is possible. */
    if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX))
        return STR2INT_OVERFLOW;
    if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN))
        return STR2INT_UNDERFLOW;
    if (*end != '\0')
        return STR2INT_INCONVERTIBLE;
    *out = l;
    return STR2INT_SUCCESS;
}

void heapify(void* fn)
{
	printf("heapify fn %p at hptr %p\n", fn, hptr);
	struct foo { codeptr fn ;} foo1;
	foo1.fn = fn;
	memcpy(hptr, &foo1, sizeof(void*));
	//((codeptr) hptr)(); // test code
	//printf("heapified %p\n", (void*) *hptr);
	hptr += sizeof(void*);
	//hptr += 8; // fudge for now TODO remove
}

void createz(byte flags, char* zname, codeptr fn) // zname being a null-terminated string
{
	memcpy(hptr, &latest, sizeof(void*));
	latest = (dent_s*) hptr;
	hptr += sizeof(void*);

	*hptr++ = flags;
	*hptr++ = strlen(zname);
	for(int i = 0 ; i< strlen(zname); ++i) *hptr++ = toupper(zname[i]);
	//printf("createz heapifying fn %p at %p\n", fn, hptr);
	heapify(fn);
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
	return (codeptr)(dw->name + dw->len);
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
        token = strtok_r(rest, " \n", &rest); 
	strupr(token);
        return token;
}

/*
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
		//if('a'<= c && c <= 'z') c = c + 'A' - 'a';
		wordpad[++count] = toupper(c);
		if(c == '\n' || c == ' ' || c == EOF || c == 0) {count--; break; }
	}
	wordpad[0] = count;
	wordpad[count+2] = 0;

}



void p_nextword () { p_bl(); p_word(); p_find(); }
*/

void p_words() {
	dent_s* dw = latest;
	while(dw) {
		for(int i = 0; i < dw->len; ++i) putchar(dw->name[i]);
		puts("");
		dw = dw->prev;
	}
	// TODO
}

void p_dots()
{
	printf("Stack: (%d):", tos);
	for(int i = 0; i< tos; ++i) printf("%ld ", dstack[i]);
}

void p_plus()
{
	//cell_t v1 = pop();
	//cell_t v2 = pop();
	push(pop() + pop());
}

void p_dot()
{
	printf("%ld ", pop());

}

void docol()
{
	puts("docol TODO NOW tricky!");
	
}

void p_exit()
{
}

void p_colon()
{
	word();
	printf("colon:<%s>\n", token);
	createz(0, token, docol);
	heapify(p_plus); // TODO NOW we'll need to generalise
	heapify(p_exit);
}

typedef struct {byte flags; char* zname; codeptr fn; } prim_s;
prim_s prims[] =  {
	{0, ":", p_colon},
	{0, ".", p_dot},
	{0, "WORDS", p_words},
	{0, ".S", p_dots},
	{0, "+", p_plus},
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

// a debugging routine
void echo_dict() 
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


void process_word()
{
	//printf("<%s>\n", token);
	//p_word();
	dent_s* dw = find(token);
	if(dw == 0) {
		int v;
		//wordpad[wordpad[0]+1] = 0;
		str2int_errno res = str2int(&v, token, 10);
		if(res == STR2INT_SUCCESS) {
			printf("v=%d\n", v);
			push(v);
		} else { 
			printf("undefined word:<%s>\n", token);
		}
	} else {
		//printf("main finds word at dw %p\n", dw);

		//void* h = *code(dw);
		codeptr fn ;
		memcpy(&fn, code(dw), sizeof(void*));
		//printf("main: -p_words = %p , codeptr found = %p at heap location %p\n", p_words, fn, code(dw));
		fn();
	}
}

int main()
{
	int8_t STATE = 0; // 0 means interpretting
	add_primitives();
	//echo_dict();
	//p_words(); // TODO remove
	        while(fgets(tib, sizeof(tib), stdin)) {
                rest = tib;
                while(word()) process_word();
                puts("  ok");
        }

	return 0;
}
