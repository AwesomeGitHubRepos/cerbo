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


char _PAD[136]; // The input buffer
char* PAD = _PAD;



/* leave the ASCII value for space (DEC 32) on the stack
 */
void BL()
{
	PUSH(32);
}

void FIND()
{
	_PAD[_PAD[0]+1] = 0;
	printf("TODO: finding:%s", PAD+1);
}

/* read the next work from stdin and store it on the stack as address.
 * The delimiter is stored in the PAD.
 * The first char stores the number of characters, excluding the
 * delimiter.
 *
 * TODO check for PAD overflow
 * */
void WORD()
{
	char delim = POP();
	//printf("delim=%d, %d\n", delim, (char) pstack[0]);
	char n = 0;
	int c;
	for(;;) {
		c = getc(stdin);
		PAD[++n] = (char) c;
		if(c == EOF || c == '\n') break;
		if((char) c == delim) {
			//puts("bingo");
		       	break;
		}
	}
	PAD[0] = n-1;
	PUSH(PAD);
}

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

ACCEPT: // read a line from keyboard
	//fgets(TIB, sizeof(TIB), stdin);

INTERPRET:
	BL();
	WORD();
	FIND();
	if(STATE == 0) puts(" ok");
	goto QUIT;

	return 0;
}
