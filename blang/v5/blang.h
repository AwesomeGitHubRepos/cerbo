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

typedef std::function<prim_t()> func_t;

typedef std::variant<prim_t, func_t> pnode_t; // parse node
#define YYSTYPE pnode_t
extern YYSTYPE yylval;
inline pnode_t top_prog_node;

//int do_hello();
//inline std::mapyyp<std::string, func_t> funcmap = { {"hello", do_hello"}};
extern std::map<std::string, func_t> funcmap;

pnode_t make_funcall(pnode_t& identifier, int TODO);
std::string enstr(const pnode_t& pnode);


extern int yylex();

