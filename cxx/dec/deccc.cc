#include <decimal/decimal>
#include <iostream>
#include <iomanip> // for std::setprecision()

#include "dec.h"


void print_num(Num n)
{
	std::decimal::decimal64 d64(n);
	std::cout << std::setprecision(20) << std::decimal::decimal_to_float(d64) << std::endl;
	// Output is 0.20000000298023223877

}

int main()
{
	Num n = get_num();
	print_num(n);

	std::cout << ( 0.1 + 0.2 == 0.3) << std::endl; // false (0)
	std::cout << ( 0.1DD + 0.2DD == 0.3DD) << std::endl; // true (1)

	return 0;
}
