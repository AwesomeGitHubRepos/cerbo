/* tests for existence of decimal
 */

#include <iostream>

int main()
{
	bool exists = false;
#ifdef _GLIBCXX_USE_DECIMAL_FLOAT
	exists = true;
#endif

	std::cout << "decimal exists:" << exists << "\n";
	return 0;
}

