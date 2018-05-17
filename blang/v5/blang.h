#pragma once

#include <any>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>
using namespace std::literals;

typedef std::variant<double, std::string> value_t;
typedef std::vector<value_t> values;

using fn_t = std::function<value_t()>;
using fn1_t = std::function<value_t(values vs)>;

inline std::vector<std::any> parsevec;


value_t do_pi(values vs);
typedef std::map<std::string, fn1_t> funcmap_t;

inline funcmap_t funcmap = { {"pi", do_pi}};


extern int yylex();

int make_concat(int str1, int str2);
int make_num(int pos);
int make_funcall(int funid, int argid);
