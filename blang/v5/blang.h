#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#define YYSTYPE int
extern YYSTYPE yylval;



void trace(std::string text);
void emit(std::string text);

extern int yylex();

extern int top; // the top node

typedef struct {int type; int arg1; int arg2; int arg3; } tac;
int add_tac(int type, int arg1, int arg2);
int add_tac(int type, int arg1, int arg2, int arg3);
