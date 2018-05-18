#pragma once

#include <any>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>
using namespace std::literals;

typedef std::variant<double, std::string> prim_t;
typedef std::variant<prim_t> pnode_t; // parse node
#define YYSTYPE pnode_t
extern YYSTYPE yylval;
inline pnode_t top_prog_node;

extern int yylex();

