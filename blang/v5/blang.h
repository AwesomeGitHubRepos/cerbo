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
typedef std::vector<prim_t> prims_t;

typedef std::function<prim_t(prims_t)> func_t;

class funcall_c;
typedef std::variant<prim_t, funcall_c> pnode_t; // parse node
typedef std::vector<pnode_t> pnodes_t;
class funcall_c { public: std::string function_name;  pnodes_t pnodes; };

#define YYSTYPE pnode_t
extern YYSTYPE yylval;
inline pnode_t top_prog_node;

extern std::map<std::string, func_t> funcmap;

pnode_t make_funcall(pnode_t& identifier, int TODO);
std::string enstr(const pnode_t& pnode);
std::string enstr(const prim_t& prim);


extern int yylex();

