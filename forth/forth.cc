//#include <ctype.h>
//#include <cstddef>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef size_t cell_t;
static_assert(sizeof(cell_t) == sizeof(char*));

size_t IP;	// Interpreter Pointer. 

cell_t pstack[10];
cell_t *PSP = pstack; // Parameter Stack, aka data stack, aka SP
#define PUSH(x) *PSP++ = (cell_t) x // push an item onto the parameter stack
#define POP()   *--PSP //pop an item off the parameter stack

size_t RSP[10]; // Return Stack Pointer, aka RP
uint8_t heap[10000];
uint8_t* UP = heap;	// User Pointer. Holds the base address of the task's user area
size_t W;	// Working register.  Multi-purpose
size_t X;	// Working register


char _TIB[136]; // The input buffer
char* TIB = _TIB;
int bytes_read = 0; // number of bytes read into TIB



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
void p_word()
{
	puts("TODO WORD");
	char delim = POP();
	while(
	//printf("delim=%d, %d\n", delim, (char) pstack[0]);
	//char n = 0;
	//int c;
	//PUSH(PAD);
}


void p_query()
{
	//TIB[0] = ' '; // need to reserve a space so that `word' words
	in = 0; 
	bytes_read = 0;
	int c;
	for(;;) {
		c = getc(stdin);
		if(c == EOF || c == '\n' || bytes_read >= sizeof(TIB)) break;
		TIB[bytes_read++] = (char) c;
	}
	//TIB[bytes_read] = 0;
}

void p_nextword () { p_bl(); p_word(); p_find(); }

void add_primitives()
{
	//UP[0] = 0;
	//UP++;
	*UP++ = 0;
	char name[] = "hello";
	int len = strlen(name);
	*UP++ = len;
	for(int i = 0; i<len; ++i) {
		char c = name[i];
		if('a' <= c && c <= 'z') c += 'A' - 'a';
		*UP++ = c;
	}


}

int main()
{
	int8_t STATE = 0; // 0 means interpretting
	//std::byte STATE = 0; // 0 means interpretting

COLD: // initialises user variables from startup table and does ABORT
	add_primitives();

ABORT: // resets parameter stack point and do QUIT

QUIT: /* reset the return stack pointer, loop stack pointer and
	 interpret state, and begin to interpret Forth commands.
	 It is the "top level" of Forth
	 */

QUERY: // read a line from keyboard
	p_query();

INTERPRET:
	p_nextword();
	if(STATE == 0) puts(" ok");
	goto QUIT;

	return 0;
}
