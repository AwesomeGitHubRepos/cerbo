#pragma once

#include <string>
#include <variant>
#include <vector>

// bennett p173
#define TAC_ADD  1
#define TAC_ARG 12

extern int yylex();
extern void yyerror(const char *);
extern int yyparse();
extern char* yytext;

void note(const char* msg);
void xnote(const char* msg);


// from bennett p54
struct tac;
typedef std::variant<int, std::string, struct tac*> tacarg;

typedef struct tac
{	
	int op;
	tacarg args[3];
	//tacarg a;
	//tacarg b;
	//tacarg c;
} tac_t;
typedef tac_t* tacptr;


inline std::vector<tac_t> tacs;
inline int tacs_idx = 0;

#define YYSTYPE tacptr

YYSTYPE mkint(std::string);
YYSTYPE mkbin(int, YYSTYPE, YYSTYPE);
