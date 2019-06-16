#include <stdio.h>
#include <string.h>

char tib[132];
char* token;
char* rest;
char* word()
{
	token = strtok_r(rest, " \n", &rest); 
	return token;
}

int main()
{

	while(fgets(tib, sizeof(tib), stdin)) {
		rest = tib;
		while(word()) 
			printf("<%s>\n", token);
		puts("  ok");
	}

	/*
	//char str[] = "     hello    world\nhow are you";
	char tib[132];
again:
	if(ret==NULL) goto finis;
	char* token;
	char* rest = tib;
	while(token = strtok_r(rest, " \n", &rest)) {
	}
goto again;
finis:
*/
	return 0;
}
