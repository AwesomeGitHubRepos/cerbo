#include <cassert>
#include <functional>
#include <iostream>
#include <tgmath.h> // for pow

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

double endouble(const prim_t prim)
{
	return std::get<double>(prim);
}

/*
double do_add(prims_t ps)
{
	double p1 = endouble(ps[0]);
	double p2 = endouble(ps[1]);
	return p1+p2;
}
double do_sub(prims_t ps)
{
	double p1 = endouble(ps[0]);
	double p2 = endouble(ps[1]);
	return p1-p2;
}
*/

double mathop(std::function<double(double,double)> fop, const prims_t& ps)
{
	double arg1 = endouble(ps[0]);
	double arg2 = endouble(ps[1]);
	return fop(arg1, arg2);
}

double do_add(prims_t ps) { return mathop(std::plus<double>(), ps); }
double do_sub(prims_t ps) { return mathop(std::minus<double>(), ps); }
double do_mul(prims_t ps) { return mathop(std::multiplies<double>(), ps); }
double do_div(prims_t ps) { return mathop(std::divides<double>(), ps); }
double do_pow(prims_t ps) { return mathop(pow, ps); }

double do_pi(prims_t ps) { return 3.14159265359; }

int do_print(prims_t ps)
{
	for(const auto& p: ps)
		cout << enstr(p);
	//assert(ps.size()>0);
	//cout << enstr(ps[0]) << "\n";
}

int do_hello(prims_t ps) { cout << "hello world\n" ; return 0; }

std::map<std::string, func_t> funcmap = { 
	{"+",		do_add},
	{"-",		do_sub},
	{"*",		do_mul},
	{"/",		do_div},
	{"^",		do_pow},
	{"hello", do_hello},
	{"pi",		do_pi},
	{"print", do_print}
};
