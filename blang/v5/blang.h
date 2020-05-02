#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

typedef unsigned char byte_t;
#define YYSTYPE std::vector<byte_t>
#include "blang.tab.hh"



//extern YYSTYPE yylval;
inline YYSTYPE bcode;
//typedef int yytokentype;


typedef std::variant<float, std::string> val_t;
typedef struct {std::string name; val_t value; } var_t;
inline std::vector<var_t> vars;
inline std::vector<var_t> labels;

void trace(std::string text);
void emit(std::string text);

extern int yylex();

//std::vector<byte_t> to_bvec(int i);
YYSTYPE to_bvec(int i);

extern int top; // the top node

YYSTYPE join(YYSTYPE vec1, YYSTYPE vec2);
YYSTYPE join(YYSTYPE vec1, YYSTYPE vec2, YYSTYPE vec3);
YYSTYPE join(YYSTYPE vec1, YYSTYPE vec2, YYSTYPE vec3, YYSTYPE vec4);
YYSTYPE join(byte_t b, YYSTYPE vec1, YYSTYPE vec2);
YYSTYPE join_toke(yytokentype toke, YYSTYPE vec1);
YYSTYPE join_toke(yytokentype toke, int i);
YYSTYPE join_toke(yytokentype toke, YYSTYPE vec1, YYSTYPE vec2);
YYSTYPE make_kstr(YYSTYPE str);

