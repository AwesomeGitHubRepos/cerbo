#pragma once

#ifdef __cplusplus
#include <decimal/decimal>
typedef std::decimal::decimal64::__decfloat64 Num;
#else
typedef _Decimal64 Num;
#endif

#ifdef __cplusplus
extern "C" {
#endif

Num get_num();
void print_num(Num n);

#ifdef __cplusplus
}
#endif
