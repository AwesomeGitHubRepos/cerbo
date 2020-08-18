#pragma once

#include <string>
#include <variant>
#include <vector>

// bennett p173
#define TAC_ADD  	1
#define TAC_SUB		2
#define TAC_ARG 	12
#define TAC_PRINT 	13
#define TAC_CONS	14
#define TAC_CAT		15

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

YYSTYPE join_tac(YYSTYPE, YYSTYPE);
YYSTYPE mkint(std::string);
YYSTYPE mkstr(const std::string&);
YYSTYPE mkbin(int, YYSTYPE, YYSTYPE);
YYSTYPE mkstm(int, YYSTYPE);

void syntax(const char* msg);
std::string str(const tacarg& arg);
