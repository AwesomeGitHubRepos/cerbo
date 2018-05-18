#include <cassert>
#include <iostream>

#include "blang.h"

using std::cout;

std::string enstr(const pnode_t& pnode)
{

	if(std::holds_alternative<func_t>(pnode)) 
		return "func()";

	assert(std::holds_alternative<prim_t>(pnode));

	prim_t prim = std::get<prim_t>(pnode);
	if(std::holds_alternative<double>(prim))
		return std::to_string(std::get<double>(prim));

	return std::get<std::string>(prim);
}

int do_hello() { cout << "hello world\n" ; }

std::map<std::string, func_t> funcmap = { {"hello", do_hello}};
