#pragma once

#include <any>
#include <functional>
#include <memory>
#include <string>
#include <variant>
#include <vector>
using namespace std::literals;

typedef std::variant<double, std::string> value_t;
using fn_t = std::function<value_t()>;
inline std::vector<std::any> parsevec;
extern int yylex();

int make_concat(int str1, int str2);
int make_num(int pos);
