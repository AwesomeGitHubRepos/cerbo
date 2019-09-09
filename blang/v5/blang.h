#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

typedef unsigned char byte_t;

#define YYSTYPE std::vector<byte_t>
extern YYSTYPE yylval;
inline YYSTYPE bcode;



void trace(std::string text);
void emit(std::string text);

extern int yylex();

extern int top; // the top node

/*
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    HALT = 1,
    STATEMENT,
    DENT,
    TEXT,
    INTEGER,
    JUST,
    LRB,
    RRB,
    PLUS,
    MUL,
    DIV,
    POW,
    SEMI,
    IF,
    THEN,
    ELSE,
    FI,
    PRINT,
    SUB,
    UMINUS
  };
#endif
*/
