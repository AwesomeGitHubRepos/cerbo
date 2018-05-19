#include <cassert>
#include <iostream>

#include "blang.h"

using std::cout;

std::string enstr(const prim_t& prim)
{
	if(std::holds_alternative<double>(prim))
		return std::to_string(std::get<double>(prim));

	return std::get<std::string>(prim);
}

std::string enstr(const pnode_t& pnode)
{

	if(std::holds_alternative<funcall_c>(pnode)) 
		return "func()";

	assert(std::holds_alternative<prim_t>(pnode));
	return enstr(std::get<prim_t>(pnode));
}

int do_print(prims_t ps)
{
	for(const auto& p: ps)
		cout << enstr(p);
	//assert(ps.size()>0);
	//cout << enstr(ps[0]) << "\n";
}

int do_hello(prims_t ps) { cout << "hello world\n" ; return 0; }

std::map<std::string, func_t> funcmap = { 
	{"hello", do_hello},
	{"print", do_print}
};
