int main()
{
COLD: // initialises user variables from startup table and does ABORT

ABORT: // resets parameter stack point and do QUIT

QUIT: /* reset the return stack pointer, loop stack pointer and
	 interpret state, and begin to interpret Forth commands.
	 It is the "top level" of Forth
	 */

	return 0;
}
