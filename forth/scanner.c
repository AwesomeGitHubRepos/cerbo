#include <stdbool.h>
#include <stdint.h>
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

bool int_str(char*s, int64_t *v)
{
	//bool ok = false;	
	*v = 0;
	int64_t sgn = 1;
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

void repl()
{
	while(fgets(tib, sizeof(tib), stdin)) {
		rest = tib;
		while(word()) 
			printf("<%s>\n", token);
		puts("  ok");
	}
}


void try(char *s)
{
	bool ok;
	int64_t v;
	ok = int_str(s, &v);
	printf("%s: %d %ld\n", s, ok, v);	
}
void test_str()
{
	try("hello");
	try("42");
	try("666.3");
	try("-1337");
	try("+1337");
}

int main()
{

	//repl();
	test_str();

	return 0;
}
