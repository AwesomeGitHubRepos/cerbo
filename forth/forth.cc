#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
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
	for(;;) {
		p_word();

		printf("word is:<");
		for(int i = 1; i <= wordpad[0]; ++i) printf("%c", wordpad[i]); // debug purposes
		printf(">\n");

		if(wordpad[wordpad[0]+1] == '\n') puts("  ok");
	}
	return 0;
}
