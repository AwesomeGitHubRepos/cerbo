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
class pnodes_c;
typedef std::variant<prim_t, funcall_c, pnodes_c> pnode_t; // parse node

class pnodes_c { 
	public:  
		std::vector<pnode_t> pnodes; 
		void append(const pnode_t& pnode);
};

class funcall_c { public: std::string function_name;  pnodes_c pnodes; };

#define YYSTYPE pnode_t
extern YYSTYPE yylval;
inline pnode_t top_prog_node;

extern std::map<std::string, func_t> funcmap;

pnode_t make_funcall(const std::string& function_name, const pnode_t& pnode1, const pnode_t& pnode2);
pnode_t make_funcall(pnode_t& identifier, pnode_t& pnodes);
//pnodes_c append_expr(const pnodes_c& vec, const pnode_t& expr);
pnodes_c append_expr(pnode_t& vec, const pnode_t& expr);
std::string enstr(const pnode_t& pnode);
std::string enstr(const prim_t& prim);

void trace(std::string text);
void emit(std::string text);
std::string str(pnode_t& pnode);

extern int yylex();

